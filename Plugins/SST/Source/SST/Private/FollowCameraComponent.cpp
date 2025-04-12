// Copyright 2024 NFB Makes Games. All Rights Reserved.

#include "FollowCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UFollowCameraComponent::UFollowCameraComponent(): CameraZoomDistance(0), CameraLeadMaxDistance(0), CameraLeadSpeed(0), FixLeadAtMax(false), CameraZLock(false), CameraZLockHeight(0), CameraBlendSpeed(0), CameraLeadLock(false),
                                                  CrouchHeightReduction(0), bUseAutomaticZoom(true), bUseAutomaticOffset(true),
                                                  MinGroupZoomDistance(600.0f), MaxGroupZoomDistance(1800.0f), AutoZoomFactor(3.0f),
                                                  BossWeightFactor(1.5f), bDebugMode(false), BossActor(nullptr), Owner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFollowCameraComponent::CacheOwner()
{
	if (Owner) { return; }
	Owner = Cast<ACharacter>(GetOwner());
	check(Owner != nullptr);
}

void UFollowCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유자 캐싱
	CacheOwner();

	// 플레이어 모드 초기값 설정
	CurrentZoomDistance = CameraZoomDistance;
	CurrentZLockHeight = CameraZLockHeight;
	CurrentLocationOffset = CameraLocationOffset;
	CurrentRotationOffset = CameraRotationOffset;

	// 그룹 모드 초기값 설정 - 설정된 값 그대로 사용
	CurrentGroupZoomDistance = GroupZoomDistance;
	CurrentGroupZLockHeight = GroupZLockHeight;
	CurrentGroupLocationOffset = GroupLocationOffset;
	CurrentGroupRotationOffset = GroupRotationOffset;

	// 기본적으로 플레이어 자신을 그룹에 추가
	if (Owner)
	{
		AddActorToGroup(Owner);
	}

	// 초기화 로그 출력
	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("FollowCamera 초기화:"));
		UE_LOG(LogTemp, Log, TEXT("  - 플레이어 모드: 줌=%.2f, Z높이=%.2f"),
		       CameraZoomDistance, CameraZLockHeight);
		UE_LOG(LogTemp, Log, TEXT("  - 그룹 모드: 줌=%.2f, Z높이=%.2f, 패딩=%.2f"),
		       GroupZoomDistance, GroupZLockHeight, GroupPadding);
		UE_LOG(LogTemp, Log, TEXT("  - 그룹 설정: 자동줌=%s, 자동오프셋=%s, Z고정=%s"),
		       bUseAutomaticZoom ? TEXT("ON") : TEXT("OFF"),
		       bUseAutomaticOffset ? TEXT("ON") : TEXT("OFF"),
		       GroupZLock ? TEXT("ON") : TEXT("OFF"));
		UE_LOG(LogTemp, Log, TEXT("  - 줌 범위: %.2f ~ %.2f, 줌계수=%.2f"),
		       MinGroupZoomDistance, MaxGroupZoomDistance, AutoZoomFactor);
	}
}

float UFollowCameraComponent::UpdateOrGetFloatParameter(ECameraTriggerUpdateFloatParameter Parameter, FCameraTriggerUpdateFloat* Update)
{
	if (!Owner) { CacheOwner(); }

	float ret = 0.0f;
	switch (Parameter)
	{
	case ECameraTriggerUpdateFloatParameter::ZoomDistance:
		if (Update)
		{
			SetZoomDistance(Update->Value, Update->BlendTime);
		}
		ret = CameraZoomDistance;
		break;
	case ECameraTriggerUpdateFloatParameter::MaxLeadDistance:
		if (Update)
		{
			SetMaxLeadDistance(Update->Value);
		}
		ret = CameraLeadMaxDistance;
		break;
	case ECameraTriggerUpdateFloatParameter::LeadSpeed:
		if (Update)
		{
			SetLeadSpeed(Update->Value);
		}
		ret = CameraLeadSpeed;
		break;
	case ECameraTriggerUpdateFloatParameter::ZLockHeight:
		if (Update)
		{
			SetZLockHeight(Update->Value, Update->BlendTime);
		}
		ret = CameraZLockHeight;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown float parameter: %d"), Parameter);
		break;
	}
	return ret;
}

bool UFollowCameraComponent::UpdateOrGetBoolParameter(ECameraTriggerUpdateBoolParameter Parameter, FCameraTriggerUpdateBool* Update)
{
	if (!Owner) { CacheOwner(); }

	bool ret = false;
	switch (Parameter)
	{
	case ECameraTriggerUpdateBoolParameter::ZLock:
		if (Update)
		{
			SetZLock(Update->Value);
		}
		ret = CameraZLock;
		break;
	case ECameraTriggerUpdateBoolParameter::FixLeadAtMax:
		if (Update)
		{
			SetFixLeadAtMax(Update->Value);
		}
		ret = FixLeadAtMax;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown bool parameter: %d"), Parameter);
		break;
	}
	return ret;
}

FVector UFollowCameraComponent::UpdateOrGetVectorParameter(ECameraTriggerUpdateVectorParameter Parameter, FCameraTriggerUpdateVector* Update)
{
	if (!Owner) { CacheOwner(); }

	FVector ret{};
	switch (Parameter)
	{
	case ECameraTriggerUpdateVectorParameter::LocationOffset:
		if (Update)
		{
			SetLocationOffset(Update->Value, Update->BlendTime);
		}
		ret = CameraLocationOffset;
		break;
	case ECameraTriggerUpdateVectorParameter::RotationOffset:
		if (Update)
		{
			SetRotationOffset(Update->Value, Update->BlendTime);
		}
		ret = CameraRotationOffset;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown vector parameter: %d"), Parameter);
		break;
	}
	return ret;
}

ECameraMode UFollowCameraComponent::UpdateOrGetModeParameter(ECameraTriggerUpdateModeParameter Parameter, FCameraTriggerUpdateMode* Update)
{
	ECameraMode ret = ECameraMode::Player;

	switch (Parameter)
	{
	case ECameraTriggerUpdateModeParameter::CameraMode:
		if (Update)
		{
			SetCameraMode(Update->Value);
		}
		ret = CurrentCameraMode;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown mode parameter: %d"), Parameter);
		break;
	}

	return ret;
}

void UFollowCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner) { CacheOwner(); }

	// 현재 모드에 따라 다른 로직 적용
	if (CurrentCameraMode == ECameraMode::Player)
	{
		// 원래의 Player 모드 로직

		// Compute camera offset to lead based on velocity
		if (!CameraLeadLock)
		{
			float target = FixLeadAtMax ? Owner->GetActorForwardVector().X : Owner->GetVelocity().X / Owner->GetCharacterMovement()->GetMaxSpeed();
			target *= CameraLeadMaxDistance;
			CurrentLeadOffsetX = FMath::FInterpTo(CurrentLeadOffsetX, target, DeltaTime, CameraLeadSpeed);
		}

		// Update other interpolating values
		CurrentZoomDistance = FMath::FInterpTo(CurrentZoomDistance, CameraZoomDistance, DeltaTime, ZoomDistanceBlendSpeed);
		CurrentZLockHeight = FMath::FInterpTo(CurrentZLockHeight, CameraZLockHeight, DeltaTime, ZLockHeightBlendSpeed);
		CurrentLocationOffset = FMath::VInterpTo(CurrentLocationOffset, CameraLocationOffset, DeltaTime, LocationOffsetBlendSpeed);
		CurrentRotationOffset = FMath::VInterpTo(CurrentRotationOffset, CameraRotationOffset, DeltaTime, RotationOffsetBlendSpeed);

		// Move camera
		FVector targetLocation = Owner->GetActorLocation();
		FVector currentLocation = GetComponentLocation();
		targetLocation.X += CurrentLeadOffsetX + CurrentLocationOffset.X;
		targetLocation.Y += CurrentZoomDistance + CurrentLocationOffset.Y;
		float crouchOffset = Owner->bIsCrouched ? CrouchHeightReduction : 0.0f;
		targetLocation.Z = CameraZLock ? CurrentZLockHeight : (targetLocation.Z + CurrentLocationOffset.Z + crouchOffset);
		SetWorldLocation(targetLocation);

		// Rotate camera
		FVector adjustedRotation = FVector(CurrentRotationOffset.Y, CurrentRotationOffset.X, CurrentRotationOffset.Z);
		adjustedRotation.Z -= 90.0f;
		SetWorldRotation(FQuat::MakeFromEuler(adjustedRotation));
	}
	else if (CurrentCameraMode == ECameraMode::Group)
	{
		// Group 모드 로직
		CalculateGroupCameraPosition(DeltaTime);
	}
}

// 카메라 모드 설정 메서드 구현
void UFollowCameraComponent::SetCameraMode(ECameraMode NewMode)
{
	if (CurrentCameraMode != NewMode)
	{
		CurrentCameraMode = NewMode;

		// 모드 변경 시 블렌딩을 위한 설정
		if (CurrentCameraMode == ECameraMode::Group)
		{
			// 블렌딩 속도 설정 (빠른 변화를 위해)
			GroupZoomDistanceBlendSpeed = 2.0f;
			GroupZLockHeightBlendSpeed = 2.0f;
			GroupLocationOffsetBlendSpeed = 2.0f;
			GroupRotationOffsetBlendSpeed = 2.0f;

			if (bDebugMode)
			{
				UE_LOG(LogTemp, Log, TEXT("카메라 모드 변경: 그룹 모드 (%d 액터)"), GroupActors.Num());
				UE_LOG(LogTemp, Log, TEXT("  목표 줌 거리: %.2f, 현재 보간중 값: %.2f"),
				       GroupZoomDistance, CurrentGroupZoomDistance);
				UE_LOG(LogTemp, Log, TEXT("  목표 Z축 높이: %.2f, 현재 보간중 값: %.2f"),
				       GroupZLockHeight, CurrentGroupZLockHeight);
			}
		}
		else if (CurrentCameraMode == ECameraMode::Player)
		{
			// 플레이어 모드 블렌딩 속도 설정
			ZoomDistanceBlendSpeed = 2.0f;
			ZLockHeightBlendSpeed = 2.0f;
			LocationOffsetBlendSpeed = 2.0f;
			RotationOffsetBlendSpeed = 2.0f;

			if (bDebugMode)
			{
				UE_LOG(LogTemp, Log, TEXT("카메라 모드 변경: 플레이어 모드"));
				UE_LOG(LogTemp, Log, TEXT("  목표 줌 거리: %.2f, 현재 보간중 값: %.2f"),
				       CameraZoomDistance, CurrentZoomDistance);
				UE_LOG(LogTemp, Log, TEXT("  목표 Z축 높이: %.2f, 현재 보간중 값: %.2f"),
				       CameraZLockHeight, CurrentZLockHeight);
			}
		}
	}
}

// 그룹 액터 관리 메서드 구현
void UFollowCameraComponent::AddActorToGroup(AActor* ActorToAdd)
{
	if (ActorToAdd && !GroupActors.Contains(ActorToAdd))
	{
		GroupActors.Add(ActorToAdd);

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Log, TEXT("그룹에 액터 추가: %s (현재 총 %d개)"),
			       *ActorToAdd->GetName(), GroupActors.Num());
		}
	}
}

void UFollowCameraComponent::RemoveActorFromGroup(AActor* ActorToRemove)
{
	if (ActorToRemove)
	{
		GroupActors.Remove(ActorToRemove);

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Log, TEXT("그룹에서 액터 제거: %s (현재 총 %d개)"),
			       *ActorToRemove->GetName(), GroupActors.Num());
		}
	}
}

void UFollowCameraComponent::ClearGroupActors()
{
	GroupActors.Empty();

	// 소유자는 항상 그룹에 포함
	if (Owner)
	{
		AddActorToGroup(Owner);
	}

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("그룹 액터 초기화 완료 (소유자만 포함)"));
	}
}

void UFollowCameraComponent::AddActorToTrack(AActor* ActorToAdd)
{
	// 기존 그룹에 액터 추가
	AddActorToGroup(ActorToAdd);

	// 추가 추적 목록에도 추가
	if (ActorToAdd && !AdditionalActorsToTrack.Contains(ActorToAdd))
	{
		AdditionalActorsToTrack.Add(ActorToAdd);
	}
}

void UFollowCameraComponent::RemoveActorToTrack(AActor* ActorToRemove)
{
	// 기존 그룹에서 액터 제거
	RemoveActorFromGroup(ActorToRemove);

	// 추가 추적 목록에서도 제거
	if (ActorToRemove)
	{
		AdditionalActorsToTrack.Remove(ActorToRemove);
	}
}

void UFollowCameraComponent::SetBossActor(AActor* NewBossActor)
{
	if (NewBossActor)
	{
		// 기존 보스가 있었다면 그룹에서 제거
		if (BossActor && BossActor != NewBossActor)
		{
			RemoveActorFromGroup(BossActor);
		}

		// 새 보스 설정
		BossActor = NewBossActor;

		// 보스를 그룹에 추가 (추적 대상에 포함)
		AddActorToGroup(BossActor);

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Log, TEXT("보스 액터 설정: %s"), *BossActor->GetName());
		}
	}
	else
	{
		// 보스가 있었다면 그룹에서 제거
		if (BossActor)
		{
			RemoveActorFromGroup(BossActor);
			BossActor = nullptr;

			if (bDebugMode)
			{
				UE_LOG(LogTemp, Log, TEXT("보스 액터 제거"));
			}
		}
	}
}

// 새로 추가된 함수들
void UFollowCameraComponent::SetUseAutomaticZoom(bool bEnable)
{
	bUseAutomaticZoom = bEnable;

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("자동 줌 %s"), bEnable ? TEXT("활성화") : TEXT("비활성화"));
	}
}

void UFollowCameraComponent::SetUseAutomaticOffset(bool bEnable)
{
	bUseAutomaticOffset = bEnable;

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("자동 위치 조정 %s"), bEnable ? TEXT("활성화") : TEXT("비활성화"));
	}
}

void UFollowCameraComponent::SetBossWeightFactor(float NewFactor)
{
	BossWeightFactor = FMath::Max(1.0f, NewFactor);

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("보스 가중치 설정: %.2f"), BossWeightFactor);
	}
}

void UFollowCameraComponent::SetAutoZoomFactor(float NewFactor)
{
	AutoZoomFactor = FMath::Max(1.0f, NewFactor);

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("자동 줌 계수 설정: %.2f"), AutoZoomFactor);
	}
}

// 그룹 모드 설정 메서드 구현
void UFollowCameraComponent::SetGroupZoomDistance_Implementation(float Value, float BlendTime)
{
	GroupZoomDistance = Value;
	GroupZoomDistanceBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("그룹 줌 거리 설정: %.2f, 블렌드 속도: %.2f"),
		       GroupZoomDistance, GroupZoomDistanceBlendSpeed);
	}
}

void UFollowCameraComponent::SetGroupPadding_Implementation(float Value)
{
	GroupPadding = Value;

	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("그룹 패딩 설정: %.2f"), GroupPadding);
	}
}

void UFollowCameraComponent::SetGroupLocationOffset_Implementation(FVector Value, float BlendTime)
{
	GroupLocationOffset = Value;
	GroupLocationOffsetBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}

void UFollowCameraComponent::SetGroupRotationOffset_Implementation(FVector Value, float BlendTime)
{
	GroupRotationOffset = Value;
	GroupRotationOffsetBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}

void UFollowCameraComponent::SetGroupZLockHeight_Implementation(float Value, float BlendTime)
{
	GroupZLockHeight = Value;
	GroupZLockHeightBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}

void UFollowCameraComponent::SetGroupZLock_Implementation(bool Value)
{
	GroupZLock = Value;
}

// 3D 바운딩 박스 계산 함수 추가
void UFollowCameraComponent::CalculateGroupBounds(FVector& OutMin, FVector& OutMax) const
{
	OutMin = FVector(MAX_FLT, MAX_FLT, MAX_FLT);
	OutMax = FVector(-MAX_FLT, -MAX_FLT, -MAX_FLT);

	bool bHasValidActor = false;

	// 모든 액터의 위치를 고려하여 3D 바운딩 박스 계산
	for (AActor* actor : GroupActors)
	{
		if (actor && IsValid(actor))
		{
			FVector actorLoc = actor->GetActorLocation();

			// 보스는 가중치를 적용하여 바운딩 박스 계산에 더 큰 영향을 줌
			if (actor == BossActor && BossWeightFactor > 1.0f)
			{
				// 보스 위치를 중심으로 더 넓은 영역 포함
				const float BossRadius = 200.0f * BossWeightFactor; // 보스 기본 반경을 가중치만큼 확장
				OutMin.X = FMath::Min(OutMin.X, actorLoc.X - BossRadius);
				OutMin.Y = FMath::Min(OutMin.Y, actorLoc.Y - BossRadius);
				OutMin.Z = FMath::Min(OutMin.Z, actorLoc.Z - BossRadius);

				OutMax.X = FMath::Max(OutMax.X, actorLoc.X + BossRadius);
				OutMax.Y = FMath::Max(OutMax.Y, actorLoc.Y + BossRadius);
				OutMax.Z = FMath::Max(OutMax.Z, actorLoc.Z + BossRadius);
			}
			else
			{
				// 일반 액터
				const float ActorRadius = 100.0f; // 일반 액터 기본 반경
				OutMin.X = FMath::Min(OutMin.X, actorLoc.X - ActorRadius);
				OutMin.Y = FMath::Min(OutMin.Y, actorLoc.Y - ActorRadius);
				OutMin.Z = FMath::Min(OutMin.Z, actorLoc.Z - ActorRadius);

				OutMax.X = FMath::Max(OutMax.X, actorLoc.X + ActorRadius);
				OutMax.Y = FMath::Max(OutMax.Y, actorLoc.Y + ActorRadius);
				OutMax.Z = FMath::Max(OutMax.Z, actorLoc.Z + ActorRadius);
			}

			bHasValidActor = true;
		}
	}

	// 유효한 액터가 없으면 소유자 위치를 기준으로 설정
	if (!bHasValidActor && Owner)
	{
		FVector ownerLoc = Owner->GetActorLocation();
		OutMin = ownerLoc - FVector(100.0f, 100.0f, 100.0f);
		OutMax = ownerLoc + FVector(100.0f, 100.0f, 100.0f);
	}
}

// 최적의 카메라 위치 계산 함수 추가
FVector UFollowCameraComponent::CalculateOptimalCameraPosition(const FVector& GroupCenter, float ZoomDistance, float InCurrentZLockHeight) const
{
	FVector cameraPos = GroupCenter;

	// Y축(언리얼 깊이)으로 ZoomDistance만큼 이동
	cameraPos.Y += ZoomDistance;

	// 보간된 CurrentZLockHeight 값을 사용
	if (GroupZLock)
	{
		cameraPos.Z = InCurrentZLockHeight; // 보간된 값 사용

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Log, TEXT("카메라 높이 설정: %.2f (GroupZLock: %s)"), InCurrentZLockHeight, GroupZLock ? TEXT("true") : TEXT("false"));
		}
	}
	else
	{
		// 기본 높이를 GroupCenter.Z에서 약간 위로 설정
		cameraPos.Z = GroupCenter.Z + 200.0f;

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Log, TEXT("카메라 높이 설정: %.2f (중심점 + 오프셋)"), cameraPos.Z);
		}
	}

	return cameraPos;
}

// 그룹 모드 계산 헬퍼 메서드 구현 (개선)
void UFollowCameraComponent::CalculateGroupCameraPosition(float DeltaTime)
{
	// 그룹 중심점 계산
	FVector groupCenter = CalculateGroupCenter();

	// 필요한 오프셋 계산 (자동 위치 조정 사용 여부에 따라)
	FVector requiredOffset = bUseAutomaticOffset ? CalculateRequiredOffset(groupCenter) : FVector::ZeroVector;

	// 필요한 줌 계산 (자동 줌 사용 여부에 따라)
	float requiredZoom = bUseAutomaticZoom ? CalculateRequiredZoom() : GroupZoomDistance;

	// 디버그 로그 출력 (강화된 로깅)
	if (bDebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("그룹 카메라 계산:"));
		UE_LOG(LogTemp, Warning, TEXT("  - 그룹 액터 수: %d"), GroupActors.Num());
		UE_LOG(LogTemp, Warning, TEXT("  - 설정된 줌 거리: %.2f"), GroupZoomDistance);
		UE_LOG(LogTemp, Warning, TEXT("  - 계산된 필요 줌: %.2f"), requiredZoom);
		UE_LOG(LogTemp, Warning, TEXT("  - 현재 줌 값: %.2f"), CurrentGroupZoomDistance);
		UE_LOG(LogTemp, Warning, TEXT("  - 설정된 Z축 높이: %.2f"), GroupZLockHeight);
		UE_LOG(LogTemp, Warning, TEXT("  - 현재 Z축 높이: %.2f"), CurrentGroupZLockHeight);
		UE_LOG(LogTemp, Warning, TEXT("  - 자동 줌: %s, 자동 오프셋: %s"), bUseAutomaticZoom ? TEXT("ON") : TEXT("OFF"), bUseAutomaticOffset ? TEXT("ON") : TEXT("OFF"));
	}

	// 보간 값 업데이트 (더 부드러운 전환을 위해 속도 조정)
	float currentZoomDiff = FMath::Abs(CurrentGroupZoomDistance - requiredZoom);
	float zoomBlendSpeed = GroupZoomDistanceBlendSpeed;
	if (currentZoomDiff > 300.0f)
	{
		// 차이가 클수록 더 빠른 보간 (최대 4배까지 - 보다 빠른 응답을 위해 증가)
		zoomBlendSpeed = GroupZoomDistanceBlendSpeed * FMath::Min(4.0f, currentZoomDiff / 300.0f);

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("  - 줌 차이 큼: %.2f, 보간 속도 증가: %.2f"),
			       currentZoomDiff, zoomBlendSpeed);
		}
	}

	// 현재 높이와 목표 높이 간 차이 확인
	float currentHeightDiff = FMath::Abs(CurrentGroupZLockHeight - GroupZLockHeight);
	float heightBlendSpeed = GroupZLockHeightBlendSpeed;
	if (currentHeightDiff > 500.0f) // 임계값 하향 조정
	{
		// 높이 차이가 클 때 보간 속도 증가 (최대 4배까지 - 더 빠른 응답을 위해)
		heightBlendSpeed = GroupZLockHeightBlendSpeed * FMath::Min(4.0f, currentHeightDiff / 500.0f);

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("  - 높이 차이 큼: %.2f, 보간 속도 증가: %.2f"),
			       currentHeightDiff, heightBlendSpeed);
		}
	}

	// 값 보간 처리 - 설정된 목표값으로 확실히 보간
	CurrentGroupZoomDistance = FMath::FInterpTo(CurrentGroupZoomDistance, requiredZoom, DeltaTime, zoomBlendSpeed);
	CurrentGroupZLockHeight = FMath::FInterpTo(CurrentGroupZLockHeight, GroupZLockHeight, DeltaTime, heightBlendSpeed);

	// 위치 오프셋 계산 및 보간
	FVector targetOffset = bUseAutomaticOffset ? (GroupLocationOffset + requiredOffset) : GroupLocationOffset;
	CurrentGroupLocationOffset = FMath::VInterpTo(CurrentGroupLocationOffset, targetOffset, DeltaTime, GroupLocationOffsetBlendSpeed);

	// 회전 오프셋 보간
	CurrentGroupRotationOffset = FMath::VInterpTo(CurrentGroupRotationOffset, GroupRotationOffset, DeltaTime, GroupRotationOffsetBlendSpeed);

	// 카메라 위치 계산
	FVector targetLocation = groupCenter;

	// X, Y 위치 설정
	targetLocation.X += CurrentGroupLocationOffset.X;
	targetLocation.Y += CurrentGroupZoomDistance + CurrentGroupLocationOffset.Y;

	// Z 위치 설정 - 중요: GroupZLock 값을 확실히 확인하고 적용
	if (GroupZLock)
	{
		// Z축 고정 사용 - 직접 CurrentGroupZLockHeight 값 적용
		targetLocation.Z = CurrentGroupZLockHeight + CurrentGroupLocationOffset.Z;

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("  - Z축 고정 적용: %.2f (현재) / %.2f (목표)"),
			       CurrentGroupZLockHeight, GroupZLockHeight);
		}
	}
	else
	{
		// Z축 고정 사용 안함 - 그룹 중심점 기준 + 오프셋
		targetLocation.Z = groupCenter.Z + 200.0f + CurrentGroupLocationOffset.Z;

		if (bDebugMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("  - Z축 고정 미사용: %.2f (그룹중심+오프셋)"), targetLocation.Z);
		}
	}

	// 최종 위치 설정
	SetWorldLocation(targetLocation);

	// 카메라 회전 설정
	FVector adjustedRotation = FVector(CurrentGroupRotationOffset.Y, CurrentGroupRotationOffset.X, CurrentGroupRotationOffset.Z);
	adjustedRotation.Z -= 90.0f;
	SetWorldRotation(FQuat::MakeFromEuler(adjustedRotation));

	// 디버깅을 위한 최종 카메라 위치 로그
	if (bDebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("  - 최종 카메라 위치: %s"), *targetLocation.ToString());
	}
}

FVector UFollowCameraComponent::CalculateGroupCenter() const
{
	FVector center = FVector::ZeroVector;
	float totalWeight = 0.0f;

	// 모든 그룹 액터의 위치 평균 계산 (가중치 적용)
	for (AActor* actor : GroupActors)
	{
		if (actor && IsValid(actor))
		{
			float weight = 1.0f;
			// 보스는 더 높은 가중치를 가짐
			if (actor == BossActor)
			{
				weight = BossWeightFactor;
			}

			center += actor->GetActorLocation() * weight;
			totalWeight += weight;
		}
	}

	if (totalWeight > 0.0f)
	{
		center /= totalWeight;
	}
	else if (Owner)
	{
		center = Owner->GetActorLocation();
	}

	return center;
}

FVector UFollowCameraComponent::CalculateRequiredOffset(const FVector& GroupCenter) const
{
	// 자동 위치 조정을 사용하지 않으면 0 반환
	if (!bUseAutomaticOffset)
	{
		return FVector::ZeroVector;
	}

	// 3D 바운딩 박스 계산
	FVector minBounds, maxBounds;
	CalculateGroupBounds(minBounds, maxBounds);

	FVector offset = FVector::ZeroVector;

	// 바운딩 박스의 중심과 그룹 중심의 차이를 오프셋으로 사용
	FVector boundsCenter = (minBounds + maxBounds) * 0.5f;

	// 주로 X축과 Z축의 오프셋만 적용 (Y축은 줌에 사용)
	offset.X = boundsCenter.X - GroupCenter.X;
	offset.Z = boundsCenter.Z - GroupCenter.Z;

	// Y축은 더 조심스럽게 적용 (너무 큰 값은 줌을 방해할 수 있음)
	float yOffset = boundsCenter.Y - GroupCenter.Y;
	offset.Y = FMath::Clamp(yOffset, -200.0f, 200.0f);

	// 디버그 로그 (필요시)
	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("계산된 오프셋: %s (바운딩 박스: %s - %s)"),
		       *offset.ToString(), *minBounds.ToString(), *maxBounds.ToString());
	}

	return offset;
}

float UFollowCameraComponent::CalculateRequiredZoom() const
{
	// 자동 줌을 사용하지 않으면 설정된 값 그대로 사용
	if (!bUseAutomaticZoom)
	{
		if (bDebugMode)
		{
			UE_LOG(LogTemp, Log, TEXT("자동 줌 비활성화 - 설정된 줌 거리 사용: %.2f"), GroupZoomDistance);
		}
		return GroupZoomDistance;
	}

	// 그룹에 액터가 없거나 1명뿐이면 기본값 사용
	if (GroupActors.Num() <= 1)
	{
		if (bDebugMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("그룹 액터 부족 (현재 %d명) - 기본 줌 거리 사용: %.2f"),
			       GroupActors.Num(), GroupZoomDistance);
		}
		return GroupZoomDistance;
	}

	// 3D 바운딩 박스 계산
	FVector minBounds, maxBounds;
	CalculateGroupBounds(minBounds, maxBounds);

	// 바운딩 박스 크기 계산
	FVector boundsSize = maxBounds - minBounds;

	// 3D 공간에서 필요한 줌 거리 계산
	// X, Z 평면의 크기와 Y 깊이를 모두 고려
	float requiredXZSize = FMath::Max(boundsSize.X, boundsSize.Z);
	float requiredYSize = boundsSize.Y;

	// 초기 줌 거리 계산 - 에디터에서 설정한 값 사용
	float baseZoom = GroupZoomDistance;

	// X-Z 크기에 따른 줌 조정 - AutoZoomFactor 적용
	float xzZoomFactor = requiredXZSize * AutoZoomFactor * 0.5f;

	// Y 깊이에 따른 추가 줌
	float yZoomFactor = requiredYSize * 0.5f;

	// 최종 줌 거리 계산 - GroupPadding 적용
	float requiredZoom = baseZoom + GroupPadding + xzZoomFactor + yZoomFactor;

	// 최소/최대 범위 적용 - 에디터에서 설정한 범위 사용
	requiredZoom = FMath::Clamp(requiredZoom, MinGroupZoomDistance, MaxGroupZoomDistance);

	// 그룹 내 액터 수에 따른 최소 줌 거리 보장
	int32 validActorCount = 0;
	for (AActor* actor : GroupActors)
	{
		if (actor && IsValid(actor))
		{
			validActorCount++;
		}
	}

	// 액터가 2개 이상이면 최소 줌 거리 보장
	if (validActorCount >= 2)
	{
		float minZoomForActorCount = MinGroupZoomDistance + (validActorCount - 1) * 100.0f;
		requiredZoom = FMath::Max(requiredZoom, minZoomForActorCount);
	}

	// 디버그 로그
	if (bDebugMode)
	{
		UE_LOG(LogTemp, Log, TEXT("자동 줌 계산 세부정보:"));
		UE_LOG(LogTemp, Log, TEXT("  - 바운딩 박스: [%s - %s] (크기: %s)"),
		       *minBounds.ToString(), *maxBounds.ToString(), *boundsSize.ToString());
		UE_LOG(LogTemp, Log, TEXT("  - 기본 줌: %.2f, 패딩: %.2f"), baseZoom, GroupPadding);
		UE_LOG(LogTemp, Log, TEXT("  - XZ 크기: %.2f, 계수: %.2f, 결과: %.2f"),
		       requiredXZSize, AutoZoomFactor, xzZoomFactor);
		UE_LOG(LogTemp, Log, TEXT("  - 유효 액터 수: %d, 최소 액터별 줌: %.2f"),
		       validActorCount, validActorCount >= 2 ? MinGroupZoomDistance + (validActorCount - 1) * 100.0f : 0.0f);
		UE_LOG(LogTemp, Log, TEXT("  - 최종 계산된 줌: %.2f (범위: %.2f ~ %.2f)"),
		       requiredZoom, MinGroupZoomDistance, MaxGroupZoomDistance);
	}

	return requiredZoom;
}

void UFollowCameraComponent::SetZoomDistance_Implementation(float Value, float BlendTime)
{
	CameraZoomDistance = Value;
	ZoomDistanceBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}

void UFollowCameraComponent::SetMaxLeadDistance_Implementation(float Value)
{
	CameraLeadMaxDistance = Value;
}

void UFollowCameraComponent::SetLeadSpeed_Implementation(float Value)
{
	CameraLeadSpeed = Value;
}

void UFollowCameraComponent::SetZLockHeight_Implementation(float Value, float BlendTime)
{
	CameraZLockHeight = Value;
	ZLockHeightBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}

void UFollowCameraComponent::SetZLock_Implementation(bool Value)
{
	CameraZLock = Value;

	// Ensure that we don't snap to a new position when turning on/off lock. 
	CurrentLocationOffset.Z = GetComponentLocation().Z - Owner->GetActorLocation().Z;
	CurrentZLockHeight = GetComponentLocation().Z;
}

void UFollowCameraComponent::SetFixLeadAtMax_Implementation(bool Value)
{
	FixLeadAtMax = Value;
}

void UFollowCameraComponent::SetLocationOffset_Implementation(FVector Value, float BlendTime)
{
	CameraLocationOffset = Value;
	LocationOffsetBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}

void UFollowCameraComponent::SetRotationOffset_Implementation(FVector Value, float BlendTime)
{
	CameraRotationOffset = Value;
	RotationOffsetBlendSpeed = BlendTime > 0.0f ? 1.0f / BlendTime : 10.0f;
}
