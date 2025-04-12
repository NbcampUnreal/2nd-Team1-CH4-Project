// #include "Character/Camera/SmashCameraTrigger.h"
//
// #include "Character/SmashCharacter.h"
// #include "Character/Camera/SmashCameraComponent.h"
// #include "GameFramework/PlayerController.h"
//
// ASmashCameraTrigger::ASmashCameraTrigger()
// {
//     // 기본 설정
//     PrimaryActorTick.bCanEverTick = false;
// }
//
// void ASmashCameraTrigger::BeginPlay()
// {
//     Super::BeginPlay();
//     
//     // 추가 초기화 작업이 필요한 경우 여기에 구현
// }
//
// void ASmashCameraTrigger::OnOverlapBegin_Implementation(class AActor* ThisActor, class AActor* OtherActor)
// {
//     // 기본 카메라 트리거 기능 실행
//     Super::OnOverlapBegin(ThisActor, OtherActor);
//     
//     // SmashCameraComponent 찾기
//     USmashCameraComponent* CameraComp = GetSmashCameraComponent(OtherActor);
//     if (CameraComp)
//     {
//         // 현재 모드 저장 (CameraTrigger 자체는 UndoAfterEndOverlap 속성을 가지고 있음)
//         float BlendTime = CustomBlendTime > 0.0f ? CustomBlendTime : CameraComp->GetDefaultCameraBlendSpeed();
//         
//         // 기존 카메라 모드를 저장하고 기본 모드로 변경 (다른 모드로 바뀌었을 때만)
//         if (CameraComp->GetCameraMode() != ECameraMode::Default)
//         {
//             CameraComp->SetCameraMode(ECameraMode::Default, BlendTime);
//         }
//     }
// }
//
// void ASmashCameraTrigger::OnOverlapEnd_Implementation(class AActor* ThisActor, class AActor* OtherActor)
// {
//     // 기본 카메라 트리거 기능 실행 (이전 설정으로 되돌리는 UndoAfterEndOverlap 적용)
//     Super::OnOverlapEnd(ThisActor, OtherActor);
//     
//     // 트리거가 UndoAfterEndOverlap를 사용하므로 추가 코드 필요 없음
//     // 기존 카메라 트리거는 이미 이전 설정으로 복원됨
// }
//
// void ASmashCameraTrigger::InitiateCameraUpdate_Implementation(AActor* Actor, bool Undo)
// {
//     // 기본 카메라 업데이트 실행
//     Super::InitiateCameraUpdate(Actor, Undo);
// }
//
// USmashCameraComponent* ASmashCameraTrigger::GetSmashCameraComponent(AActor* Actor)
// {
//     // 이미 캐싱된 참조가 있으면 반환
//     if (SmashCameraComp && SmashCameraComp->GetOwner() == Actor)
//     {
//         return SmashCameraComp;
//     }
//     
//     // Actor가 SmashCharacter인지 확인
//     if (ASmashCharacter* SmashChar = Cast<ASmashCharacter>(Actor))
//     {
//         // SmashCameraComponent 참조 가져오기
//         SmashCameraComp = SmashChar->SmashCameraComponent;
//         return SmashCameraComp;
//     }
//     
//     // 일반 컴포넌트 검색으로 찾기 시도
//     if (USmashCameraComponent* FoundComp = Actor ? Actor->FindComponentByClass<USmashCameraComponent>() : nullptr)
//     {
//         SmashCameraComp = FoundComp;
//         return SmashCameraComp;
//     }
//     
//     // 찾지 못한 경우
//     return nullptr;
// }