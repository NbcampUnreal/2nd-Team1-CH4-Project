#include "Core/SmashPlatFighterGameMode.h"

#include "EngineUtils.h"
#include "Character/SmashCharacter.h"
// FollowCameraComponent 헤더 포함 - 경로는 프로젝트에 맞게 수정하세요
#include "FollowCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ASmashPlatFighterGameMode::ASmashPlatFighterGameMode()
{
    // 초기화
    BossActor = nullptr;
}

void ASmashPlatFighterGameMode::BeginPlay()
{
	Super::BeginPlay();
    
    // 시작 시 이미 월드에 있는 플레이어들을 찾아서 등록
    for (TActorIterator<ASmashCharacter> It(GetWorld()); It; ++It)
    {
        ASmashCharacter* Character = *It;
        if (Character && Character->IsPlayerControlled())
        {
            RegisteredPlayers.AddUnique(Character);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("SmashPlatFighterGameMode: BeginPlay, Found %d initial players"), RegisteredPlayers.Num());
}

void ASmashPlatFighterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
    
    UE_LOG(LogTemp, Log, TEXT("Player logged in: %s"), *NewPlayer->GetName());

    if (NewPlayer)
    {
        // 약간의 지연을 줘서 캐릭터가 완전히 생성된 후 처리
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
        {
            if (APawn* Pawn = NewPlayer->GetPawn())
            {
                if (ASmashCharacter* SmashChar = Cast<ASmashCharacter>(Pawn))
                {
                    // 플레이어 목록에 추가
                    RegisteredPlayers.AddUnique(SmashChar);
                    
                    UE_LOG(LogTemp, Log, TEXT("Added player to registry: %s, Total: %d"), 
                           *SmashChar->GetName(), RegisteredPlayers.Num());

                    // 카메라 컴포넌트 찾기
                    UFollowCameraComponent* CameraComp = SmashChar->FindComponentByClass<UFollowCameraComponent>();
                    if (CameraComp)
                    {
                        UE_LOG(LogTemp, Log, TEXT("Found camera component for player: %s"), *SmashChar->GetName());
                        
                        // 이미 등록된 다른 플레이어들을 이 카메라에 추가
                        for (ASmashCharacter* OtherPlayer : RegisteredPlayers)
                        {
                            if (OtherPlayer != SmashChar)
                            {
                                CameraComp->AddActorToTrack(OtherPlayer);
                                UE_LOG(LogTemp, Log, TEXT("Added player %s to %s's camera"), 
                                       *OtherPlayer->GetName(), *SmashChar->GetName());
                            }
                        }

                        // 보스가 있으면 등록
                        if (BossActor)
                        {
                            CameraComp->SetBossActor(BossActor);
                            UE_LOG(LogTemp, Log, TEXT("Added boss %s to %s's camera"), 
                                   *BossActor->GetName(), *SmashChar->GetName());
                        }

                        // 다른 플레이어들의 카메라에 이 플레이어 추가
                        for (ASmashCharacter* OtherPlayer : RegisteredPlayers)
                        {
                            if (OtherPlayer != SmashChar)
                            {
                                UFollowCameraComponent* OtherCamera = OtherPlayer->FindComponentByClass<UFollowCameraComponent>();
                                if (OtherCamera)
                                {
                                    OtherCamera->AddActorToTrack(SmashChar);
                                    UE_LOG(LogTemp, Log, TEXT("Added player %s to %s's camera"), 
                                           *SmashChar->GetName(), *OtherPlayer->GetName());
                                }
                            }
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("No FollowCameraComponent found for player: %s"), 
                               *SmashChar->GetName());
                    }
                }
            }
        }, 1.0f, false);  // 1초 지연으로 캐릭터가 완전히 생성될 시간을 줍니다
    }
}

void ASmashPlatFighterGameMode::Logout(AController* Exiting)
{
    // 로그아웃한 플레이어의 캐릭터 처리
    if (Exiting)
    {
        APlayerController* PC = Cast<APlayerController>(Exiting);
        if (PC && PC->GetPawn())
        {
            ASmashCharacter* SmashChar = Cast<ASmashCharacter>(PC->GetPawn());
            if (SmashChar)
            {
                UE_LOG(LogTemp, Log, TEXT("Player logout: %s"), *SmashChar->GetName());
                
                // 플레이어 목록에서 제거
                RegisteredPlayers.Remove(SmashChar);

                // 다른 모든 플레이어의 카메라에서 이 플레이어 제거
                for (ASmashCharacter* OtherPlayer : RegisteredPlayers)
                {
                    UFollowCameraComponent* OtherCamera = OtherPlayer->FindComponentByClass<UFollowCameraComponent>();
                    if (OtherCamera)
                    {
                        OtherCamera->RemoveActorToTrack(SmashChar);
                        UE_LOG(LogTemp, Log, TEXT("Removed player %s from %s's camera"), 
                               *SmashChar->GetName(), *OtherPlayer->GetName());
                    }
                }
            }
        }
    }

	Super::Logout(Exiting);
}

void ASmashPlatFighterGameMode::RegisterBoss(AActor* NewBossActor)
{
    if (!NewBossActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to register null boss actor"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Registering boss: %s"), *NewBossActor->GetName());
    
    // 보스 액터 저장
    BossActor = NewBossActor;

    // 모든 플레이어의 카메라에 보스 등록
    for (ASmashCharacter* Player : RegisteredPlayers)
    {
        if (Player)
        {
            UFollowCameraComponent* CameraComp = Player->FindComponentByClass<UFollowCameraComponent>();
            if (CameraComp)
            {
                CameraComp->SetBossActor(BossActor);
                UE_LOG(LogTemp, Log, TEXT("Added boss to %s's camera"), *Player->GetName());
            }
        }
    }
}

void ASmashPlatFighterGameMode::SwitchToGroupMode(bool bEnableGroupMode)
{
    UE_LOG(LogTemp, Log, TEXT("Switching to %s mode"), bEnableGroupMode ? TEXT("Group") : TEXT("Player"));

    // 모든 플레이어의 카메라 모드 전환
    for (ASmashCharacter* Player : RegisteredPlayers)
    {
        if (Player)
        {
            UFollowCameraComponent* CameraComp = Player->FindComponentByClass<UFollowCameraComponent>();
            if (CameraComp)
            {
                if (bEnableGroupMode)
                {
                    // 그룹 모드로 전환
                    CameraComp->SetCameraMode(ECameraMode::Group);
                    UE_LOG(LogTemp, Log, TEXT("Set %s's camera to Group mode"), *Player->GetName());
                }
                else
                {
                    // 플레이어 모드로 전환
                    CameraComp->SetCameraMode(ECameraMode::Player);
                    UE_LOG(LogTemp, Log, TEXT("Set %s's camera to Player mode"), *Player->GetName());
                }
            }
        }
    }
}

