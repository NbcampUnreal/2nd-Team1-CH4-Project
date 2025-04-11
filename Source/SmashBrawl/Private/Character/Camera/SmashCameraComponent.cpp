#include "Character/Camera/SmashCameraComponent.h"
#include "Character/SmashCharacter.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

USmashCameraComponent::USmashCameraComponent()
	: MainTarget(nullptr)
	  , bIsMasterCamera(false)
	  , CustomCameraActor(nullptr)
	  , OriginalCameraLocation(FVector::ZeroVector)
	  , bIsCameraShaking(false)
	  , ShakeIntensity(0.0f)
	  , ShakeDuration(0.0f)
	  , ShakeTimer(0.0f)
	  , ShakeFalloff(1.0f)
	  , CurrentGroupPositionOffset(FVector::ZeroVector)
{
	// 기본 설정
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 카메라 모드 설정 - 기본값은 Default(개인 모드)로 변경
	CurrentCameraMode = ECameraMode::Default;

	// 기본 카메라 설정
	CameraZoomDistance = 800.0f;
	CameraLeadMaxDistance = 100.0f;
	CameraLeadSpeed = 3.0f;
	CameraZLock = true;
	CameraZLockHeight = 0.0f;

	// 자동 카메라 활성화
	bAutoCameraEnabled = true;

	// 네트워크 복제 설정
	SetIsReplicatedByDefault(true);
}

void USmashCameraComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 속성들 등록 (CurrentCameraMode 제외)
	DOREPLIFETIME(USmashCameraComponent, RegisteredPlayers);
	DOREPLIFETIME(USmashCameraComponent, bAutoCameraEnabled);
	DOREPLIFETIME(USmashCameraComponent, bIsMasterCamera);
	DOREPLIFETIME(USmashCameraComponent, CustomCameraActor);
}

void USmashCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// 메인 타겟 초기화 (소유자를 기본 타겟으로 설정)
	MainTarget = GetOwner();
	if (MainTarget)
	{
		AddTargetActor(MainTarget);
	}

	// 초기 위치 저장 (카메라 흔들림용)
	OriginalCameraLocation = GetComponentLocation();

	// 첫 번째 생성된 카메라를 마스터 카메라로 설정 (서버만)
	if (GetOwner()->HasAuthority() && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		bool bIsOnlyCamera = true;

		// 모든 SmashCharacter를 검색하여 다른 카메라가 있는지 확인
		for (TActorIterator<ASmashCharacter> It(GetWorld()); It; ++It)
		{
			ASmashCharacter* Character = *It;
			if (Character && Character != GetOwner())
			{
				if (USmashCameraComponent* OtherCamera = Character->FindComponentByClass<USmashCameraComponent>())
				{
					bIsOnlyCamera = false;
					break;
				}
			}
		}

		if (bIsOnlyCamera)
		{
			// 이 카메라가 유일하다면 마스터 카메라로 설정
			BecomesMasterCamera();
		}
	}
}

bool USmashCameraComponent::IsLocalPlayerCamera() const
{
	// 소유자가 유효한지 확인
	if (!GetOwner())
	{
		return false;
	}

	// 로컬 플레이어 컨트롤러 가져오기
	APlayerController* LocalController = GetWorld()->GetFirstPlayerController();
	if (!LocalController)
	{
		return false;
	}

	// 로컬 플레이어의 캐릭터가 이 카메라의 소유자인지 확인
	return LocalController->GetPawn() == GetOwner();
}

void USmashCameraComponent::InitializeCameraForPlayer(bool bGroupModeForHost)
{
	// 모든 캐릭터를 타겟으로 등록 (그룹 모드 사용 시 필요)
	TArray<AActor*> AllCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmashCharacter::StaticClass(), AllCharacters);

	for (AActor* Actor : AllCharacters)
	{
		if (Actor != GetOwner())
		{
			AddTargetActor(Actor);
		}
	}

	// 로컬 플레이어가 호스트인지 확인
	bool bIsHost = GetOwner()->HasAuthority() && IsLocalPlayerCamera();

    // 호스트와 클라이언트 모두 그룹 모드로 설정
		SetCameraMode(ECameraMode::Group, 1.0f);
    
    if (bIsHost && bGroupModeForHost)
    {
		UE_LOG(LogTemp, Display, TEXT("호스트: 카메라 모드가 그룹 모드로 초기화되었습니다."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("클라이언트: 카메라 모드가 그룹 모드로 초기화되었습니다."));

        // 메인 타겟 설정 코드는 유지 (카메라 위치 계산의 기준점으로 사용)
		SetMainTarget(GetOwner());
	}

	// 로컬 플레이어 컨트롤러의 뷰 타겟을 자신의 카메라로 설정
	if (IsLocalPlayerCamera())
	{
		APlayerController* PC = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
		if (PC)
		{
			SetPlayerViewTarget(PC, 1.0f);
		}
	}
}

void USmashCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 카메라 모드에 따라 다른 업데이트 로직 실행
	switch (CurrentCameraMode)
	{
	case ECameraMode::Default:
		UpdateDefaultCamera(DeltaTime);
		break;
	case ECameraMode::Group:
		UpdateGroupCamera(DeltaTime);
		break;
	case ECameraMode::Custom:
		UpdateCustomCamera(DeltaTime);
		break;
	default:
		UpdateDefaultCamera(DeltaTime);
		break;
	}

	// 카메라 흔들림 효과 업데이트
	if (bIsCameraShaking)
	{
		UpdateCameraShake(DeltaTime);
	}

	// 자동 카메라 모드 활성화 시 최적의 뷰 계산
	if (bAutoCameraEnabled && CurrentCameraMode == ECameraMode::Group)
	{
		UpdateOptimalCameraView();
	}

	// 마스터 카메라인 경우 추가 업데이트
	if (bIsMasterCamera)
	{
		UpdateMasterCamera();
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USmashCameraComponent::CacheOwner()
{
	if (Owner)
	{
		return;
	}

	Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
	{
		// 치명적 오류 대신 경고만 출력하고 계속 진행
		UE_LOG(LogTemp, Warning, TEXT("SmashCameraComponent: Owner is not a valid Character!"));
		return;
	}
}

void USmashCameraComponent::SetCameraMode(ECameraMode NewMode, float BlendTime)
{
	// 이미 같은 모드면 아무것도 하지 않음
	if (CurrentCameraMode == NewMode)
	{
		return;
	}

	// 카메라 모드 설정 (로컬에서만 적용)
	CurrentCameraMode = NewMode;

	// 모드에 따른 카메라 설정 적용
	ApplyCameraModeSettings(NewMode, BlendTime);

	// 로컬 플레이어의 카메라인 경우 뷰 타겟 설정
	if (IsLocalPlayerCamera())
	{
		APlayerController* PC = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
		if (PC)
		{
			// 커스텀 카메라가 있고 커스텀 모드인 경우
			if (CustomCameraActor && NewMode == ECameraMode::Custom)
			{
				PC->SetViewTargetWithBlend(CustomCameraActor, BlendTime);
			}
			else
			{
				// 그 외의 경우 자신을 뷰 타겟으로 설정
				PC->SetViewTargetWithBlend(GetOwner(), BlendTime);
			}
		}
	}
}

void USmashCameraComponent::ApplyCameraModeSettings(ECameraMode Mode, float BlendTime)
{
	switch (Mode)
	{
	case ECameraMode::Default:
		// 개인 모드 설정
		SetZoomDistance(800.0f, BlendTime);
		SetLeadSpeed(3.0f);
		SetMaxLeadDistance(100.0f);
		SetZLock(true);
		SetZLockHeight(0.0f, BlendTime);
		SetLocationOffset(FVector(0, 0, 100.0f), BlendTime);
		SetRotationOffset(FVector(0, 0, 0), BlendTime);
		break;

	case ECameraMode::Group:
		// 그룹 모드 설정
		SetZoomDistance(MinGroupZoomDistance, BlendTime);
		SetLeadSpeed(1.5f);
		SetMaxLeadDistance(50.0f);
		SetZLock(true);
		SetZLockHeight(0.0f, BlendTime);
		SetLocationOffset(FVector(0, 0, 200.0f), BlendTime);
		SetRotationOffset(FVector(0, 0, 0), BlendTime);
		break;

	case ECameraMode::Custom:
		// Custom 모드는 EnterCustomMode에서 프리셋을 직접 적용
		break;
	}
}

void USmashCameraComponent::AddTargetActor(AActor* TargetActor)
{
	if (!TargetActor || TargetActors.Contains(TargetActor))
	{
		return;
	}

	TargetActors.Add(TargetActor);

	// 첫 번째 등록된 액터를 메인 타겟으로 설정
	if (!MainTarget)
	{
		MainTarget = TargetActor;
	}
}

void USmashCameraComponent::RemoveTargetActor(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return;
	}

	TargetActors.Remove(TargetActor);

	// 메인 타겟이 제거되면 다른 타겟으로 변경
	if (MainTarget == TargetActor)
	{
		if (TargetActors.Num() > 0)
		{
			MainTarget = TargetActors[0];
		}
		else
		{
			MainTarget = nullptr;
		}
	}
}

void USmashCameraComponent::ClearTargets()
{
	TargetActors.Empty();
	MainTarget = nullptr;
}

void USmashCameraComponent::SetMainTarget(AActor* NewMainTarget)
{
	if (!NewMainTarget)
	{
		return;
	}

	// 타겟 목록에 없으면 추가
	if (!TargetActors.Contains(NewMainTarget))
	{
		TargetActors.Add(NewMainTarget);
	}

	MainTarget = NewMainTarget;
}

void USmashCameraComponent::ShakeCamera(float Intensity, float Duration, float Falloff)
{
	bIsCameraShaking = true;
	ShakeIntensity = Intensity;
	ShakeDuration = Duration;
	ShakeTimer = 0.0f;
	ShakeFalloff = Falloff;

	// 흔들림 시작 시 현재 위치 저장
	OriginalCameraLocation = GetComponentLocation();
}

void USmashCameraComponent::ZoomToTargets(float BlendTime)
{
	// 최적의 줌 거리 계산
	float OptimalZoom = CalculateOptimalZoomDistance();

	// 계산된 거리로 줌 설정
	SetZoomDistance(OptimalZoom, BlendTime);
}

void USmashCameraComponent::TransitionToPreset(const FCameraPreset& Preset)
{
	// 프리셋의 설정을 카메라에 적용
	SetZoomDistance(Preset.ZoomDistance, Preset.BlendTime);
	SetMaxLeadDistance(Preset.LeadMaxDistance);
	SetLeadSpeed(Preset.LeadSpeed);
	SetZLock(Preset.ZLock);
	SetZLockHeight(Preset.ZLockHeight, Preset.BlendTime);
	SetLocationOffset(Preset.LocationOffset, Preset.BlendTime);
	SetRotationOffset(Preset.RotationOffset, Preset.BlendTime);
}

void USmashCameraComponent::EnterCustomMode(const FCameraPreset& Preset, float BlendTime)
{
	// 현재 모드 저장
	PreviousCameraMode = CurrentCameraMode;

	// 커스텀 모드로 전환
	SetCameraMode(ECameraMode::Custom, BlendTime);

	// 프리셋 적용
	TransitionToPreset(Preset);
}

void USmashCameraComponent::ExitCustomMode(float BlendTime)
{
	// 이전 모드로 복원
	SetCameraMode(PreviousCameraMode, BlendTime);
}

void USmashCameraComponent::BecomesMasterCamera()
{
	// 권한 확인 (서버만 설정 가능)
	if (GetOwner()->HasAuthority())
	{
		// 이미 마스터 카메라면 아무것도 하지 않음
		if (bIsMasterCamera)
		{
			return;
		}

		// 기존 마스터 카메라가 있으면 해제
		for (TActorIterator<ASmashCharacter> It(GetWorld()); It; ++It)
		{
			ASmashCharacter* Character = *It;
			if (Character && Character != GetOwner())
			{
				USmashCameraComponent* CameraComp = Character->FindComponentByClass<USmashCameraComponent>();
				if (CameraComp && CameraComp->IsMasterCamera())
				{
					CameraComp->StopBeingMasterCamera();
				}
			}
		}

		// 이 카메라를 마스터 카메라로 설정
		bIsMasterCamera = true;

		// 모든 클라이언트에게 전파
		Multicast_BecomesMasterCamera();

		// 마스터 카메라 설정이 독립적인 카메라 제어에 영향을 주지 않음
		// 각 플레이어는 자신의 뷰 타겟 유지
	}
	else
	{
		// 클라이언트는 서버에 요청
		Server_BecomesMasterCamera();
	}
}

void USmashCameraComponent::StopBeingMasterCamera()
{
	// 마스터 카메라가 아니면 아무것도 하지 않음
	if (!bIsMasterCamera)
	{
		return;
	}

	// 마스터 카메라 상태 해제
	bIsMasterCamera = false;
}

void USmashCameraComponent::Server_BecomesMasterCamera_Implementation()
{
	BecomesMasterCamera();
}

void USmashCameraComponent::Multicast_BecomesMasterCamera_Implementation()
{
	// 서버에서는 이미 설정되었으므로 클라이언트만 처리
	if (!GetOwner()->HasAuthority())
	{
		bIsMasterCamera = true;
	}
}

void USmashCameraComponent::SetPlayerViewTarget(APlayerController* PlayerController, float BlendTime)
{
	if (!PlayerController)
	{
		return;
	}

	// 커스텀 카메라가 있고 커스텀 모드인 경우
	if (CustomCameraActor && CurrentCameraMode == ECameraMode::Custom)
	{
		PlayerController->SetViewTargetWithBlend(CustomCameraActor, BlendTime);
	}
	else
	{
		// 소유자를 뷰 타겟으로 설정
		PlayerController->SetViewTargetWithBlend(GetOwner(), BlendTime);
	}
}

void USmashCameraComponent::SetAllPlayersViewTarget(float BlendTime)
{
	// 이 함수는 사용하지 않음 - 각 플레이어가 자신의 카메라를 독립적으로 제어
	// 마스터 카메라가 여러 플레이어의 뷰를 제어하지 않음
}

void USmashCameraComponent::SetCustomCameraActor(ACameraActor* CustomCamera, float BlendTime)
{
	// 권한 확인 (서버만 설정 가능)
	if (GetOwner()->HasAuthority())
	{
		// 외부 카메라 설정
		CustomCameraActor = CustomCamera;

		// 이전 모드 저장 (트리거처럼 동작하도록)
		PreviousCameraMode = CurrentCameraMode;

		// 커스텀 모드로 설정
		SetCameraMode(ECameraMode::Custom, BlendTime);

		// 모든 클라이언트에게 전파
		Multicast_SetCustomCameraActor(CustomCamera, BlendTime);
	}
	else
	{
		// 클라이언트는 서버에 요청
		Server_SetCustomCameraActor(CustomCamera, BlendTime);
	}
}

void USmashCameraComponent::ResetToDefaultView(float BlendTime)
{
	// 커스텀 카메라 해제
	CustomCameraActor = nullptr;

	// 이전 모드로 복원
	SetCameraMode(PreviousCameraMode, BlendTime);
}

void USmashCameraComponent::Server_SetCustomCameraActor_Implementation(ACameraActor* CustomCamera, float BlendTime)
{
	SetCustomCameraActor(CustomCamera, BlendTime);
}

void USmashCameraComponent::Multicast_SetCustomCameraActor_Implementation(ACameraActor* CustomCamera, float BlendTime)
{
	// 서버에서는 이미 설정되었으므로 클라이언트만 처리
	if (!GetOwner()->HasAuthority())
	{
		CustomCameraActor = CustomCamera;

		// 로컬 플레이어의 카메라인 경우 새 모드 적용
		if (IsLocalPlayerCamera())
		{
			// 이전 모드 저장
			PreviousCameraMode = CurrentCameraMode;

			// 커스텀 모드로 변경
			SetCameraMode(ECameraMode::Custom, BlendTime);
		}
	}
}

void USmashCameraComponent::RegisterPlayer(ASmashCharacter* Player)
{
	if (!Player || RegisteredPlayers.Contains(Player))
	{
		return;
	}

	RegisteredPlayers.Add(Player);

	// 카메라 타겟으로 추가 (그룹 모드에서 사용)
	AddTargetActor(Player);
}

void USmashCameraComponent::UnregisterPlayer(ASmashCharacter* Player)
{
	if (!Player || !RegisteredPlayers.Contains(Player))
	{
		return;
	}

	// 카메라 타겟에서 제거
	RemoveTargetActor(Player);

	// 플레이어 목록에서 제거
	RegisteredPlayers.Remove(Player);
}

void USmashCameraComponent::SetAutoCameraEnabled(bool bEnabled)
{
	bAutoCameraEnabled = bEnabled;

	// 자동 모드 설정 변경은 이제 로컬 카메라 모드에만 영향
	if (IsLocalPlayerCamera())
	{
		if (bEnabled && RegisteredPlayers.Num() > 1 && GetOwner()->HasAuthority())
		{
			// 호스트이고 여러 플레이어가 있으면 그룹 모드로 전환 가능
			SetCameraMode(ECameraMode::Group);
		}
		else if (bEnabled && !GetOwner()->HasAuthority())
		{
			// 클라이언트는 항상 개인 모드
			SetCameraMode(ECameraMode::Default);
		}
	}
}

// 카메라 모드별 업데이트 함수
void USmashCameraComponent::UpdateDefaultCamera(float DeltaTime)
{
	// 기본 카메라 모드 - 단일 타겟(MainTarget) 추적
	// Super::TickComponent에서 기본 카메라 로직 처리
}

void USmashCameraComponent::UpdateGroupCamera(float DeltaTime)
{
	if (TargetActors.Num() <= 1)
	{
		// 타겟이 하나 이하면 기본 모드처럼 동작
		UpdateDefaultCamera(DeltaTime);
		return;
	}

	// 모든 타겟의 평균 위치 계산
	FVector TargetCenterPos = FVector::ZeroVector;
	int32 ValidTargetCount = 0;

	// 경계 상자 계산을 위한 초기값
	FVector MinBound(MAX_FLT, MAX_FLT, MAX_FLT);
	FVector MaxBound(-MAX_FLT, -MAX_FLT, -MAX_FLT);

	for (AActor* Target : TargetActors)
	{
		if (IsValid(Target))
		{
			FVector TargetPos = Target->GetActorLocation();

			// 평균 위치 계산을 위한 합산
			TargetCenterPos += TargetPos;
			ValidTargetCount++;

			// 경계 상자 업데이트
			MinBound.X = FMath::Min(MinBound.X, TargetPos.X);
			MinBound.Y = FMath::Min(MinBound.Y, TargetPos.Y);
			MinBound.Z = FMath::Min(MinBound.Z, TargetPos.Z);

			MaxBound.X = FMath::Max(MaxBound.X, TargetPos.X);
			MaxBound.Y = FMath::Max(MaxBound.Y, TargetPos.Y);
			MaxBound.Z = FMath::Max(MaxBound.Z, TargetPos.Z);
		}
	}

	if (ValidTargetCount > 0)
	{
		// 평균 위치 계산
		TargetCenterPos /= ValidTargetCount;

		// 경계 상자 크기 계산
		FVector BoundSize = MaxBound - MinBound;

		// 최적의 줌 거리 계산
		float OptimalZoom = FMath::Clamp(
			BoundSize.X * 0.8f + TargetPadding, // X축 사용 (사이드뷰 게임)
			MinGroupZoomDistance,
			MaxGroupZoomDistance
		);

		// 부드러운 줌 적용
		float NewZoomDistance = FMath::FInterpTo(
			CameraZoomDistance,
			OptimalZoom,
			DeltaTime,
			ZoomSmoothingFactor
		);
		CameraZoomDistance = NewZoomDistance;

		// 타겟 중심 위치로 보간
		FVector DesiredOffset = FVector(0, 0, 0);

		// X축: 플레이어들의 평균 X 위치
		if (MainTarget)
		{
			DesiredOffset.X = TargetCenterPos.X - MainTarget->GetActorLocation().X;

			// Z축: 플레이어들의 평균 Z 위치
			if (!CameraZLock)
			{
				DesiredOffset.Z = TargetCenterPos.Z - MainTarget->GetActorLocation().Z;
			}

			// 부드러운 포지션 보간
			CurrentGroupPositionOffset = FMath::VInterpTo(
				CurrentGroupPositionOffset,
				DesiredOffset,
				DeltaTime,
				2.0f
			);

			// 최종 위치 오프셋 적용
			CameraLocationOffset = FVector(
				CurrentGroupPositionOffset.X,
				CameraLocationOffset.Y,
				CameraLocationOffset.Z
			);
		}
	}
}

void USmashCameraComponent::UpdateCustomCamera(float DeltaTime)
{
	// 커스텀 카메라 모드는 외부에서 설정한 값을 사용하므로 여기서는 추가 작업 없음
}

float USmashCameraComponent::CalculateOptimalZoomDistance() const
{
	if (TargetActors.Num() <= 1)
	{
		return MinGroupZoomDistance;
	}

	// 모든 타겟의 경계 상자 계산
	FVector MinBound(MAX_FLT, MAX_FLT, MAX_FLT);
	FVector MaxBound(-MAX_FLT, -MAX_FLT, -MAX_FLT);

	for (AActor* Target : TargetActors)
	{
		if (IsValid(Target))
		{
			FVector TargetPos = Target->GetActorLocation();

			MinBound.X = FMath::Min(MinBound.X, TargetPos.X);
			MinBound.Y = FMath::Min(MinBound.Y, TargetPos.Y);
			MinBound.Z = FMath::Min(MinBound.Z, TargetPos.Z);

			MaxBound.X = FMath::Max(MaxBound.X, TargetPos.X);
			MaxBound.Y = FMath::Max(MaxBound.Y, TargetPos.Y);
			MaxBound.Z = FMath::Max(MaxBound.Z, TargetPos.Z);
		}
	}

	// 경계 상자 크기 계산
	FVector BoundSize = MaxBound - MinBound;

	// 사이드뷰 게임이므로 X축 거리를 기준으로 줌 계산
	float OptimalZoom = FMath::Clamp(
		BoundSize.X * 0.8f + TargetPadding,
		MinGroupZoomDistance,
		MaxGroupZoomDistance
	);

	return OptimalZoom;
}

void USmashCameraComponent::UpdateCameraShake(float DeltaTime)
{
	// 카메라 흔들림 타이머 업데이트
	ShakeTimer += DeltaTime;

	if (ShakeTimer >= ShakeDuration)
	{
		// 흔들림 종료
		bIsCameraShaking = false;
		return;
	}

	// 남은 시간에 따른 감쇠 계산
	float RemainingPct = 1.0f - (ShakeTimer / ShakeDuration);
	float CurrentIntensity = ShakeIntensity * FMath::Pow(RemainingPct, ShakeFalloff);

	// 랜덤 오프셋 계산
	FVector ShakeOffset;
	ShakeOffset.X = FMath::RandRange(-1.0f, 1.0f) * CurrentIntensity;
	ShakeOffset.Y = FMath::RandRange(-1.0f, 1.0f) * CurrentIntensity;
	ShakeOffset.Z = FMath::RandRange(-1.0f, 1.0f) * CurrentIntensity;

	// 원래 위치에 흔들림 오프셋 적용
	// 이 값은 최종적으로 Super::TickComponent에서 처리됨
	CameraLocationOffset += ShakeOffset;
}

void USmashCameraComponent::UpdateOptimalCameraView()
{
	// 그룹 모드인 경우에만 최적 줌 적용
	if (CurrentCameraMode == ECameraMode::Group && TargetActors.Num() > 1)
	{
		ZoomToTargets();
	}
}

void USmashCameraComponent::UpdateMasterCamera()
{
	// 마스터 카메라 특별 작업 (필요시 구현)
	// 이제 마스터 카메라는 특정 이벤트와 트리거에 대한 권한만 가지며
	// 다른 플레이어의 카메라 뷰를 제어하지 않음
}

void USmashCameraComponent::Client_AddTargetActor_Implementation(AActor* TargetActor)
{
	if (TargetActor && TargetActor != GetOwner() && !TargetActors.Contains(TargetActor))
	{
		AddTargetActor(TargetActor);
	}
}

void USmashCameraComponent::OnRep_RegisteredPlayers()
{
	// RegisteredPlayers가 변경될 때 TargetActors 업데이트
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player && Player != GetOwner() && !TargetActors.Contains(Player))
		{
			AddTargetActor(Player);
		}
	}
}
