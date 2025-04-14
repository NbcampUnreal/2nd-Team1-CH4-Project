// #include "Character/Camera/SmashCameraComponent.h"
// #include "Character/SmashCharacter.h"
// #include "GameFramework/Character.h"
// #include "DrawDebugHelpers.h"
// #include "EngineUtils.h"
// #include "Engine/World.h"
// #include "GameFramework/PlayerController.h"
// #include "Kismet/GameplayStatics.h"
// #include "Net/UnrealNetwork.h"
//
// // 디버그 로그 출력 매크로 정의
// #define SMASH_CAMERA_LOG(Format, ...) \
//     if (bEnableDebugLogging) { \
//         UE_LOG(LogTemp, Display, TEXT("[SmashCamera] %s"), *FString::Printf(Format, ##__VA_ARGS__)); \
//     }
//
// USmashCameraComponent::USmashCameraComponent()
//     : MainTarget(nullptr)
//     , bIsMasterCamera(false)
//     , OriginalCameraLocation(FVector::ZeroVector)
//     , bIsCameraShaking(false)
//     , ShakeIntensity(0.0f)
//     , ShakeDuration(0.0f)
//     , ShakeTimer(0.0f)
//     , ShakeFalloff(1.0f)
//     , CurrentGroupPositionOffset(FVector::ZeroVector)
// {
//     // 기본 설정
//     PrimaryComponentTick.bCanEverTick = true;
//
//     // 기본 카메라 모드 설정
//     CurrentCameraMode = ECameraMode::Default;
//     PreviousCameraMode = ECameraMode::Default;
//
//     // 기본 카메라 설정
//     CameraZoomDistance = 800.0f;
//     CameraLeadMaxDistance = 100.0f;
//     CameraLeadSpeed = 3.0f;
//     CameraZLock = true;
//     CameraZLockHeight = 0.0f;
//
//     // 자동 카메라 활성화
//     bAutoCameraEnabled = true;
//     
//     // 디버그 로깅
//     bEnableDebugLogging = false;
//
//     // 네트워크 복제 설정
//     SetIsReplicatedByDefault(true);
// }
//
// void USmashCameraComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
//     Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//     // 복제할 속성들 등록 (CurrentCameraMode 제외 - 로컬에서 독립적으로 설정)
//     DOREPLIFETIME(USmashCameraComponent, RegisteredPlayers);
//     DOREPLIFETIME(USmashCameraComponent, bAutoCameraEnabled);
//     DOREPLIFETIME(USmashCameraComponent, bIsMasterCamera);
// }
//
// void USmashCameraComponent::BeginPlay()
// {
//     Super::BeginPlay();
//
//     // 메인 타겟 초기화 (소유자를 기본 타겟으로 설정)
//     MainTarget = GetOwner();
//     if (MainTarget)
//     {
//         AddTargetActor(MainTarget);
//     }
//
//     // 초기 위치 저장 (카메라 흔들림용)
//     OriginalCameraLocation = GetComponentLocation();
//
//     // 첫 번째 생성된 카메라를 마스터 카메라로 설정 (서버만)
//     if (GetOwner()->HasAuthority() && GetOwner()->GetLocalRole() == ROLE_Authority)
//     {
//         bool bIsOnlyCamera = true;
//
//         // 모든 SmashCharacter를 검색하여 다른 카메라가 있는지 확인
//         for (TActorIterator<ASmashCharacter> It(GetWorld()); It; ++It)
//         {
//             ASmashCharacter* Character = *It;
//             if (Character && Character != GetOwner())
//             {
//                 if (USmashCameraComponent* OtherCamera = Character->FindComponentByClass<USmashCameraComponent>())
//                 {
//                     bIsOnlyCamera = false;
//                     break;
//                 }
//             }
//         }
//
//         if (bIsOnlyCamera)
//         {
//             // 이 카메라가 유일하다면 마스터 카메라로 설정
//             BecomesMasterCamera();
//         }
//     }
//     
//     SMASH_CAMERA_LOG(TEXT("카메라 컴포넌트 초기화 완료: 모드=%s, 줌거리=%.1f, Z축고정=%s, Z축높이=%.1f"), 
//         *UEnum::GetValueAsString(CurrentCameraMode), 
//         CameraZoomDistance, 
//         CameraZLock ? TEXT("True") : TEXT("False"), 
//         CameraZLockHeight);
// }
//
// bool USmashCameraComponent::IsLocalPlayerCamera() const
// {
//     // 소유자가 유효한지 확인
//     if (!GetOwner())
//     {
//         return false;
//     }
//
//     // 로컬 플레이어 컨트롤러 가져오기
//     APlayerController* LocalController = GetWorld()->GetFirstPlayerController();
//     if (!LocalController)
//     {
//         return false;
//     }
//
//     // 로컬 플레이어의 캐릭터가 이 카메라의 소유자인지 확인
//     return LocalController->GetPawn() == GetOwner();
// }
//
// void USmashCameraComponent::InitializeCameraForPlayer(bool bGroupModeForHost)
// {
//     // 모든 캐릭터를 타겟으로 등록 (그룹 모드 사용 시 필요)
//     TArray<AActor*> AllCharacters;
//     UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmashCharacter::StaticClass(), AllCharacters);
//
//     for (AActor* Actor : AllCharacters)
//     {
//         if (Actor != GetOwner())
//         {
//             AddTargetActor(Actor);
//         }
//     }
//
//     // 로컬 플레이어가 호스트인지 확인
//     bool bIsHost = GetOwner()->HasAuthority() && IsLocalPlayerCamera();
//
//     // 호스트와 클라이언트 모두 그룹 모드로 설정 (요청에 따라 변경)
//     SetCameraMode(ECameraMode::Group, 1.0f);
//     
//     if (bIsHost && bGroupModeForHost)
//     {
//         SMASH_CAMERA_LOG(TEXT("호스트: 카메라 모드가 그룹 모드로 초기화되었습니다."));
//     }
//     else
//     {
//         SMASH_CAMERA_LOG(TEXT("클라이언트: 카메라 모드가 그룹 모드로 초기화되었습니다."));
//
//         // 메인 타겟 설정 (위치 계산 기준점으로 사용)
//         SetMainTarget(GetOwner());
//     }
//
//     // 로컬 플레이어 컨트롤러의 뷰 타겟을 자신의 카메라로 설정
//     if (IsLocalPlayerCamera())
//     {
//         APlayerController* PC = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
//         if (PC)
//         {
//             SetPlayerViewTarget(PC, 1.0f);
//         }
//     }
// }
//
// void USmashCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
//     // 모드에 따라 다른 처리 로직 적용
//     if (CurrentCameraMode == ECameraMode::Default)
//     {
//         // Default 모드: 부모 클래스의 TickComponent 호출하여 기본 기능 활용
//         Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//         
//         // 카메라 흔들림 효과 업데이트 (필요시)
//         if (bIsCameraShaking)
//         {
//             UpdateCameraShake(DeltaTime);
//         }
//     }
//     else // Group 모드
//     {
//         // Group 모드: 커스텀 로직으로 다중 타겟 추적
//         UpdateGroupCamera(DeltaTime);
//         
//         // 카메라 흔들림 효과 업데이트 (Group 모드에서도 적용)
//         if (bIsCameraShaking)
//         {
//             UpdateCameraShake(DeltaTime);
//         }
//     }
//
//     // 자동 카메라 모드 활성화 시 최적의 뷰 계산 (Group 모드에서만)
//     if (bAutoCameraEnabled && CurrentCameraMode == ECameraMode::Group)
//     {
//         UpdateOptimalCameraView();
//     }
//
//     // 마스터 카메라인 경우 추가 업데이트
//     if (bIsMasterCamera)
//     {
//         UpdateMasterCamera();
//     }
// }
//
// void USmashCameraComponent::CacheOwner()
// {
//     if (Owner)
//     {
//         return;
//     }
//
//     Owner = Cast<ACharacter>(GetOwner());
//     if (!Owner)
//     {
//         // 치명적 오류 대신 경고만 출력하고 계속 진행
//         UE_LOG(LogTemp, Warning, TEXT("SmashCameraComponent: Owner is not a valid Character!"));
//         return;
//     }
// }
//
// void USmashCameraComponent::SetCameraMode(ECameraMode NewMode, float BlendTime)
// {
//     // 이미 같은 모드면 아무것도 하지 않음
//     if (CurrentCameraMode == NewMode)
//     {
//         return;
//     }
//
//     SMASH_CAMERA_LOG(TEXT("카메라 모드 변경: %s -> %s (BlendTime: %.2f)"), 
//         *UEnum::GetValueAsString(CurrentCameraMode), 
//         *UEnum::GetValueAsString(NewMode), 
//         BlendTime);
//
//     // 이전 모드 저장
//     PreviousCameraMode = CurrentCameraMode;
//
//     // 카메라 모드 설정 (로컬에서만 적용)
//     CurrentCameraMode = NewMode;
//
//     // 모드에 따른 카메라 설정 적용
//     ApplyCameraModeSettings(NewMode, BlendTime);
//
//     // 로컬 플레이어의 카메라인 경우 뷰 타겟 설정
//     if (IsLocalPlayerCamera())
//     {
//         APlayerController* PC = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
//         if (PC)
//         {
//             // 소유자를 뷰 타겟으로 설정
//             PC->SetViewTargetWithBlend(GetOwner(), BlendTime);
//         }
//     }
// }
//
// void USmashCameraComponent::ApplyCameraModeSettings(ECameraMode Mode, float BlendTime)
// {
//     // 블렌드 시간이 0 이하이면 최소값으로 설정 (0으로 나누기 방지)
//     float SafeBlendTime = FMath::Max(BlendTime, 0.01f);
//
//     switch (Mode)
//     {
//     case ECameraMode::Default:
//         {
//             // 개인 모드 설정 - 부모 클래스 값 직접 설정
//             float DefaultZoom = 800.0f;
//             float DefaultLeadSpeed = 3.0f;
//             float DefaultMaxLeadDistance = 100.0f;
//             float DefaultZLockHeight = 0.0f;
//             FVector DefaultLocationOffset = FVector(0, 0, 100.0f);
//             FVector DefaultRotationOffset = FVector(0, 0, 0);
//             
//             // 보간 속도 계산 (BlendTime의 역수)
//             float ZoomBlendSpeed = 1.0f / SafeBlendTime;
//             float ZLockBlendSpeed = 1.0f / SafeBlendTime;
//             float LocationBlendSpeed = 1.0f / SafeBlendTime;
//             float RotationBlendSpeed = 1.0f / SafeBlendTime;
//             
//             // 직접 부모 클래스 함수 호출
//             SetZoomDistance(DefaultZoom, SafeBlendTime);
//             SetLeadSpeed(DefaultLeadSpeed);
//             SetMaxLeadDistance(DefaultMaxLeadDistance);
//             SetZLock(true);
//             SetZLockHeight(DefaultZLockHeight, SafeBlendTime);
//             SetLocationOffset(DefaultLocationOffset, SafeBlendTime);
//             SetRotationOffset(DefaultRotationOffset, SafeBlendTime);
//             
//             SMASH_CAMERA_LOG(TEXT("Default 모드 설정 적용: 줌=%.1f, 리드속도=%.1f, 최대리드거리=%.1f, Z고정=%s, Z높이=%.1f"),
//                 DefaultZoom, DefaultLeadSpeed, DefaultMaxLeadDistance, TEXT("True"), DefaultZLockHeight);
//         }
//         break;
//
//     case ECameraMode::Group:
//         {
//             // 그룹 모드 설정
//             float GroupZoom = MinGroupZoomDistance;
//             float GroupLeadSpeed = 1.5f;
//             float GroupMaxLeadDistance = 50.0f;
//             FVector GroupLocationOffset = FVector(0, 0, 200.0f);
//             FVector GroupRotationOffset = FVector(0, 0, 0);
//             
//             // 부모 클래스 함수 호출
//             SetZoomDistance(GroupZoom, SafeBlendTime);
//             SetLeadSpeed(GroupLeadSpeed);
//             SetMaxLeadDistance(GroupMaxLeadDistance);
//             SetZLock(true);
//             SetZLockHeight(0.0f, SafeBlendTime);
//             SetLocationOffset(GroupLocationOffset, SafeBlendTime);
//             SetRotationOffset(GroupRotationOffset, SafeBlendTime);
//             
//             SMASH_CAMERA_LOG(TEXT("Group 모드 설정 적용: 줌=%.1f, 리드속도=%.1f, 최대리드거리=%.1f, Z고정=%s, Z높이=%.1f"),
//                 GroupZoom, GroupLeadSpeed, GroupMaxLeadDistance, TEXT("True"), 0.0f);
//         }
//         break;
//     }
// }
//
// void USmashCameraComponent::AddTargetActor(AActor* TargetActor)
// {
//     if (!TargetActor || TargetActors.Contains(TargetActor))
//     {
//         return;
//     }
//
//     TargetActors.Add(TargetActor);
//     SMASH_CAMERA_LOG(TEXT("타겟 추가: %s (현재 타겟 수: %d)"), *TargetActor->GetName(), TargetActors.Num());
//
//     // 첫 번째 등록된 액터를 메인 타겟으로 설정
//     if (!MainTarget)
//     {
//         MainTarget = TargetActor;
//         SMASH_CAMERA_LOG(TEXT("메인 타겟 설정: %s"), *MainTarget->GetName());
//     }
// }
//
// void USmashCameraComponent::RemoveTargetActor(AActor* TargetActor)
// {
//     if (!TargetActor)
//     {
//         return;
//     }
//
//     if (TargetActors.Contains(TargetActor))
//     {
//         TargetActors.Remove(TargetActor);
//         SMASH_CAMERA_LOG(TEXT("타겟 제거: %s (현재 타겟 수: %d)"), *TargetActor->GetName(), TargetActors.Num());
//     }
//
//     // 메인 타겟이 제거되면 다른 타겟으로 변경
//     if (MainTarget == TargetActor)
//     {
//         if (TargetActors.Num() > 0)
//         {
//             MainTarget = TargetActors[0];
//             SMASH_CAMERA_LOG(TEXT("메인 타겟 변경: %s"), *MainTarget->GetName());
//         }
//         else
//         {
//             MainTarget = nullptr;
//             SMASH_CAMERA_LOG(TEXT("메인 타겟 제거: 타겟 없음"));
//         }
//     }
// }
//
// void USmashCameraComponent::ClearTargets()
// {
//     TargetActors.Empty();
//     MainTarget = nullptr;
//     SMASH_CAMERA_LOG(TEXT("모든 타겟 제거됨"));
// }
//
// void USmashCameraComponent::SetMainTarget(AActor* NewMainTarget)
// {
//     if (!NewMainTarget)
//     {
//         return;
//     }
//
//     // 타겟 목록에 없으면 추가
//     if (!TargetActors.Contains(NewMainTarget))
//     {
//         TargetActors.Add(NewMainTarget);
//         SMASH_CAMERA_LOG(TEXT("메인 타겟이 타겟 목록에 없어 추가됨: %s"), *NewMainTarget->GetName());
//     }
//
//     MainTarget = NewMainTarget;
//     SMASH_CAMERA_LOG(TEXT("메인 타겟 설정: %s"), *MainTarget->GetName());
// }
//
// void USmashCameraComponent::ShakeCamera(float Intensity, float Duration, float Falloff)
// {
//     bIsCameraShaking = true;
//     ShakeIntensity = Intensity;
//     ShakeDuration = Duration;
//     ShakeTimer = 0.0f;
//     ShakeFalloff = Falloff;
//
//     // 흔들림 시작 시 현재 위치 저장
//     OriginalCameraLocation = GetComponentLocation();
//     
//     SMASH_CAMERA_LOG(TEXT("카메라 흔들림 시작: 강도=%.2f, 지속=%.2f초, 감쇠=%.2f"), 
//         Intensity, Duration, Falloff);
// }
//
// void USmashCameraComponent::ZoomToTargets(float BlendTime)
// {
//     // 타겟이 1개 이하면 동작하지 않음
//     if (TargetActors.Num() <= 1 || CurrentCameraMode != ECameraMode::Group)
//     {
//         return;
//     }
//
//     // 최적의 줌 거리 계산
//     float OptimalZoom = CalculateOptimalZoomDistance();
//
//     // 계산된 거리로 줌 설정
//     SetZoomDistance(OptimalZoom, BlendTime);
//     
//     SMASH_CAMERA_LOG(TEXT("최적 줌 적용: %.1f (BlendTime: %.2f)"), OptimalZoom, BlendTime);
// }
//
// void USmashCameraComponent::BecomesMasterCamera()
// {
//     // 권한 확인 (서버만 설정 가능)
//     if (GetOwner()->HasAuthority())
//     {
//         // 이미 마스터 카메라면 아무것도 하지 않음
//         if (bIsMasterCamera)
//         {
//             return;
//         }
//
//         // 기존 마스터 카메라가 있으면 해제
//         for (TActorIterator<ASmashCharacter> It(GetWorld()); It; ++It)
//         {
//             ASmashCharacter* Character = *It;
//             if (Character && Character != GetOwner())
//             {
//                 USmashCameraComponent* CameraComp = Character->FindComponentByClass<USmashCameraComponent>();
//                 if (CameraComp && CameraComp->IsMasterCamera())
//                 {
//                     CameraComp->StopBeingMasterCamera();
//                 }
//             }
//         }
//
//         // 이 카메라를 마스터 카메라로 설정
//         bIsMasterCamera = true;
//         SMASH_CAMERA_LOG(TEXT("이 카메라가 마스터 카메라로 설정됨"));
//
//         // 모든 클라이언트에게 전파
//         Multicast_BecomesMasterCamera();
//
//         // 마스터 카메라 설정이 독립적인 카메라 제어에 영향을 주지 않음
//         // 각 플레이어는 자신의 뷰 타겟 유지
//     }
//     else
//     {
//         // 클라이언트는 서버에 요청
//         Server_BecomesMasterCamera();
//     }
// }
//
// void USmashCameraComponent::StopBeingMasterCamera()
// {
//     // 마스터 카메라가 아니면 아무것도 하지 않음
//     if (!bIsMasterCamera)
//     {
//         return;
//     }
//
//     // 마스터 카메라 상태 해제
//     bIsMasterCamera = false;
//     SMASH_CAMERA_LOG(TEXT("마스터 카메라 상태 해제됨"));
// }
//
// void USmashCameraComponent::Server_BecomesMasterCamera_Implementation()
// {
//     BecomesMasterCamera();
// }
//
// void USmashCameraComponent::Multicast_BecomesMasterCamera_Implementation()
// {
//     // 서버에서는 이미 설정되었으므로 클라이언트만 처리
//     if (!GetOwner()->HasAuthority())
//     {
//         bIsMasterCamera = true;
//         SMASH_CAMERA_LOG(TEXT("마스터 카메라 상태가 복제됨"));
//     }
// }
//
// void USmashCameraComponent::SetPlayerViewTarget(APlayerController* PlayerController, float BlendTime)
// {
//     if (!PlayerController)
//     {
//         return;
//     }
//
//     // 소유자를 뷰 타겟으로 설정
//     PlayerController->SetViewTargetWithBlend(GetOwner(), BlendTime);
//     SMASH_CAMERA_LOG(TEXT("플레이어 컨트롤러의 뷰 타겟 설정: %s (BlendTime: %.2f)"), 
//         *GetOwner()->GetName(), BlendTime);
// }
//
// void USmashCameraComponent::SetAllPlayersViewTarget(float BlendTime)
// {
//     // 이 함수는 사용하지 않음 - 각 플레이어가 자신의 카메라를 독립적으로 제어
//     // 마스터 카메라가 여러 플레이어의 뷰를 제어하지 않음
//     UE_LOG(LogTemp, Warning, TEXT("SetAllPlayersViewTarget 함수는 사용되지 않습니다. 각 플레이어는 자신의 카메라를 독립적으로 제어합니다."));
// }
//
// void USmashCameraComponent::RegisterPlayer(ASmashCharacter* Player)
// {
//     if (!Player || RegisteredPlayers.Contains(Player))
//     {
//         return;
//     }
//
//     RegisteredPlayers.Add(Player);
//     SMASH_CAMERA_LOG(TEXT("플레이어 등록: %s (총 %d명)"), *Player->GetName(), RegisteredPlayers.Num());
//
//     // 카메라 타겟으로 추가 (그룹 모드에서 사용)
//     AddTargetActor(Player);
// }
//
// void USmashCameraComponent::UnregisterPlayer(ASmashCharacter* Player)
// {
//     if (!Player || !RegisteredPlayers.Contains(Player))
//     {
//         return;
//     }
//
//     // 카메라 타겟에서 제거
//     RemoveTargetActor(Player);
//
//     // 플레이어 목록에서 제거
//     RegisteredPlayers.Remove(Player);
//     SMASH_CAMERA_LOG(TEXT("플레이어 등록 해제: %s (남은 플레이어: %d명)"), 
//         *Player->GetName(), RegisteredPlayers.Num());
// }
//
// void USmashCameraComponent::SetAutoCameraEnabled(bool bEnabled)
// {
//     bAutoCameraEnabled = bEnabled;
//     SMASH_CAMERA_LOG(TEXT("자동 카메라 모드: %s"), bEnabled ? TEXT("활성화") : TEXT("비활성화"));
//
//     // 자동 모드 설정 변경은 이제 로컬 카메라 모드에만 영향
//     if (IsLocalPlayerCamera())
//     {
//         if (bEnabled)
//         {
//             // 호스트이고 여러 플레이어가 있으면 그룹 모드로 전환
//             if (RegisteredPlayers.Num() > 1)
//             {
//                 SetCameraMode(ECameraMode::Group, 1.0f);
//                 SMASH_CAMERA_LOG(TEXT("자동 카메라: 그룹 모드로 전환 (플레이어 %d명)"), RegisteredPlayers.Num());
//             }
//         }
//         else
//         {
//             // 자동 모드 비활성화 시 개인 모드로 전환
//             SetCameraMode(ECameraMode::Default, 1.0f);
//             SMASH_CAMERA_LOG(TEXT("자동 카메라: 개인 모드로 전환"));
//         }
//     }
// }
//
// void USmashCameraComponent::UpdateGroupCamera(float DeltaTime)
// {
//     if (TargetActors.Num() <= 1)
//     {
//         // 타겟이 하나 이하면 부모 클래스 TickComponent 호출하여 기본 모드처럼 동작
//         Super::TickComponent(DeltaTime, ELevelTick_None, nullptr);
//         return;
//     }
//
//     // 모든 타겟의 평균 위치 계산
//     FVector TargetCenterPos = FVector::ZeroVector;
//     int32 ValidTargetCount = 0;
//
//     // 경계 상자 계산을 위한 초기값
//     FVector MinBound(MAX_FLT, MAX_FLT, MAX_FLT);
//     FVector MaxBound(-MAX_FLT, -MAX_FLT, -MAX_FLT);
//
//     for (AActor* Target : TargetActors)
//     {
//         if (IsValid(Target))
//         {
//             FVector TargetPos = Target->GetActorLocation();
//
//             // 평균 위치 계산을 위한 합산
//             TargetCenterPos += TargetPos;
//             ValidTargetCount++;
//
//             // 경계 상자 업데이트
//             MinBound.X = FMath::Min(MinBound.X, TargetPos.X);
//             MinBound.Y = FMath::Min(MinBound.Y, TargetPos.Y);
//             MinBound.Z = FMath::Min(MinBound.Z, TargetPos.Z);
//
//             MaxBound.X = FMath::Max(MaxBound.X, TargetPos.X);
//             MaxBound.Y = FMath::Max(MaxBound.Y, TargetPos.Y);
//             MaxBound.Z = FMath::Max(MaxBound.Z, TargetPos.Z);
//         }
//     }
//
//     if (ValidTargetCount > 0)
//     {
//         // 평균 위치 계산
//         TargetCenterPos /= ValidTargetCount;
//
//         // 경계 상자 크기 계산
//         FVector BoundSize = MaxBound - MinBound;
//
//         // 최적의 줌 거리 계산
//         float OptimalZoom = FMath::Clamp(
//             BoundSize.X * 0.8f + TargetPadding, // X축 사용 (사이드뷰 게임)
//             MinGroupZoomDistance,
//             MaxGroupZoomDistance
//         );
//
//         // 부드러운 줌 적용 (부모 클래스의 함수 사용)
//         if (FMath::Abs(CameraZoomDistance - OptimalZoom) > 1.0f)
//         {
//             SetZoomDistance(OptimalZoom, 0.2f); // 짧은 블렌드 시간으로 부드럽게 전환
//         }
//
//         // 타겟 중심 위치로 보간하기 위한 오프셋 계산
//         FVector DesiredOffset = FVector(0, 0, 0);
//
//         // X축: 플레이어들의 평균 X 위치
//         if (MainTarget)
//         {
//             DesiredOffset.X = TargetCenterPos.X - MainTarget->GetActorLocation().X;
//
//             // Z축: 플레이어들의 평균 Z 위치 (Z고정이 비활성화된 경우만)
//             if (!CameraZLock)
//             {
//                 DesiredOffset.Z = TargetCenterPos.Z - MainTarget->GetActorLocation().Z;
//             }
//
//             // 부드러운 포지션 보간
//             CurrentGroupPositionOffset = FMath::VInterpTo(
//                 CurrentGroupPositionOffset,
//                 DesiredOffset,
//                 DeltaTime,
//                 2.0f
//             );
//
//             // 위치 오프셋 적용 (부모 클래스의 함수 사용)
//             FVector NewLocationOffset = FVector(
//                 CurrentGroupPositionOffset.X,
//                 CameraLocationOffset.Y, // Y축은 유지 (줌 거리)
//                 CameraLocationOffset.Z  // Z축 오프셋은 유지
//             );
//             
//             // 현재 값과 다를 경우에만 업데이트 (불필요한 호출 방지)
//             if (!NewLocationOffset.Equals(CameraLocationOffset, 1.0f))
//             {
//                 SetLocationOffset(NewLocationOffset, 0.1f); // 짧은 블렌드 시간으로 부드럽게 전환
//             }
//         }
//         
//         // 부모 클래스의 기본 업데이트 처리 호출 (위치 및 회전 계산용)
//         Super::TickComponent(DeltaTime, ELevelTick_None, nullptr);
//     }
// }
//
// float USmashCameraComponent::CalculateOptimalZoomDistance() const
// {
//     if (TargetActors.Num() <= 1)
//     {
//         return MinGroupZoomDistance;
//     }
//
//     // 모든 타겟의 경계 상자 계산
//     FVector MinBound(MAX_FLT, MAX_FLT, MAX_FLT);
//     FVector MaxBound(-MAX_FLT, -MAX_FLT, -MAX_FLT);
//
//     for (AActor* Target : TargetActors)
//     {
//         if (IsValid(Target))
//         {
//             FVector TargetPos = Target->GetActorLocation();
//
//             MinBound.X = FMath::Min(MinBound.X, TargetPos.X);
//             MinBound.Y = FMath::Min(MinBound.Y, TargetPos.Y);
//             MinBound.Z = FMath::Min(MinBound.Z, TargetPos.Z);
//
//             MaxBound.X = FMath::Max(MaxBound.X, TargetPos.X);
//             MaxBound.Y = FMath::Max(MaxBound.Y, TargetPos.Y);
//             MaxBound.Z = FMath::Max(MaxBound.Z, TargetPos.Z);
//         }
//     }
//
//     // 경계 상자 크기 계산
//     FVector BoundSize = MaxBound - MinBound;
//
//     // 사이드뷰 게임이므로 X축 거리를 기준으로 줌 계산
//     float OptimalZoom = FMath::Clamp(
//         BoundSize.X * 0.8f + TargetPadding,
//         MinGroupZoomDistance,
//         MaxGroupZoomDistance
//     );
//
//     return OptimalZoom;
// }
//
// void USmashCameraComponent::UpdateCameraShake(float DeltaTime)
// {
//     // 카메라 흔들림 타이머 업데이트
//     ShakeTimer += DeltaTime;
//
//     if (ShakeTimer >= ShakeDuration)
//     {
//         // 흔들림 종료
//         bIsCameraShaking = false;
//         
//         // 보간을 통해 원래 위치로 복귀 (현재 적용 중인 오프셋 유지)
//         return;
//     }
//
//     // 남은 시간에 따른 감쇠 계산
//     float RemainingPct = 1.0f - (ShakeTimer / ShakeDuration);
//     float CurrentIntensity = ShakeIntensity * FMath::Pow(RemainingPct, ShakeFalloff);
//
//     // 랜덤 오프셋 계산
//     FVector ShakeOffset;
//     ShakeOffset.X = FMath::RandRange(-1.0f, 1.0f) * CurrentIntensity;
//     ShakeOffset.Y = FMath::RandRange(-1.0f, 1.0f) * CurrentIntensity;
//     ShakeOffset.Z = FMath::RandRange(-1.0f, 1.0f) * CurrentIntensity;
//
//     // 현재 위치 오프셋에 흔들림 효과 추가
//     FVector CurrentOffset = CameraLocationOffset;
//     FVector NewOffset = CurrentOffset + ShakeOffset;
//     
//     // 위치 오프셋 적용 (매우 짧은 블렌드 시간으로 즉시 적용에 가깝게)
//     SetLocationOffset(NewOffset, 0.01f);
// }
//
// void USmashCameraComponent::UpdateOptimalCameraView()
// {
//     // 그룹 모드인 경우에만 최적 줌 적용
//     if (CurrentCameraMode == ECameraMode::Group && TargetActors.Num() > 1)
//     {
//         // 일정 주기로 줌 거리 업데이트 (매 프레임 호출 대신 일정 간격으로)
//         static float UpdateTimer = 0.0f;
//         UpdateTimer += GetWorld()->GetDeltaSeconds();
//         
//         if (UpdateTimer >= 0.5f) // 0.5초마다 업데이트
//         {
//             ZoomToTargets(0.5f); // 0.5초 동안 부드럽게 전환
//             UpdateTimer = 0.0f;
//         }
//     }
// }
//
// void USmashCameraComponent::UpdateMasterCamera()
// {
//     // 마스터 카메라 특별 작업 (필요시 구현)
//     // 이제 마스터 카메라는 특정 이벤트와 트리거에 대한 권한만 가지며
//     // 다른 플레이어의 카메라 뷰를 제어하지 않음
// }
//
// void USmashCameraComponent::Client_AddTargetActor_Implementation(AActor* TargetActor)
// {
//     if (TargetActor && TargetActor != GetOwner() && !TargetActors.Contains(TargetActor))
//     {
//         AddTargetActor(TargetActor);
//     }
// }
//
// void USmashCameraComponent::OnRep_RegisteredPlayers()
// {
//     // RegisteredPlayers가 변경될 때 TargetActors 업데이트
//     for (ASmashCharacter* Player : RegisteredPlayers)
//     {
//         if (Player && Player != GetOwner() && !TargetActors.Contains(Player))
//         {
//             AddTargetActor(Player);
//         }
//     }
// }