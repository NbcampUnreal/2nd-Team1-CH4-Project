#include "Core/SmashPlatFighterGameMode.h"

#include "EngineUtils.h"
#include "Character/SmashCharacter.h"
#include "Character/Camera/SmashCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Core/SmashGameInstance.h"

ASmashPlatFighterGameMode::ASmashPlatFighterGameMode()
{
	// 기본 설정
}

void ASmashPlatFighterGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASmashPlatFighterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
		{
			if (APawn* Pawn = NewPlayer->GetPawn())
			{
				if (ASmashCharacter* SmashChar = Cast<ASmashCharacter>(Pawn))
				{
					// 플레이어 목록에 추가
					RegisteredPlayers.Add(SmashChar);

					// 마스터 카메라 찾기 (이벤트 처리 용도)
					if (USmashCameraComponent* MasterCamera = FindMasterCamera())
					{
						// 마스터 카메라에 플레이어 등록 (그룹 뷰용 타겟으로 추가)
						MasterCamera->RegisterPlayer(SmashChar);
					}
					else
					{
						// 마스터 카메라가 없으면 이 플레이어의 카메라를 마스터로 설정
						USmashCameraComponent* PlayerCamera = SmashChar->FindComponentByClass<USmashCameraComponent>();
						if (PlayerCamera)
						{
							PlayerCamera->BecomesMasterCamera();
						}
					}

					// 각 플레이어의 카메라 모드 초기화
					// (독립적 제어를 위해 각 클라이언트에서 적절한 모드로 설정)
					if (SmashChar->SmashCameraComponent)
					{
						// 이 함수는 내부적으로 호스트/클라이언트를 구분하여 처리
						SmashChar->InitializeCameraMode();
					}
				}
			}
		}, 1.0f, false);
	}
}

void ASmashPlatFighterGameMode::Logout(AController* Exiting)
{
	// 로그아웃한 플레이어의 캐릭터 처리
	if (Exiting)
	{
		APlayerController* PC = Cast<APlayerController>(Exiting);
		if (PC)
		{
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				ASmashCharacter* SmashChar = Cast<ASmashCharacter>(Pawn);
				if (SmashChar)
				{
					// 플레이어 목록에서 제거
					RegisteredPlayers.Remove(SmashChar);

					// 마스터 카메라 찾기
					USmashCameraComponent* MasterCamera = FindMasterCamera();
					if (MasterCamera)
					{
						// 마스터 카메라에서 플레이어 등록 해제
						MasterCamera->UnregisterPlayer(SmashChar);
					}
				}
			}
		}
	}

	Super::Logout(Exiting);
}

USmashCameraComponent* ASmashPlatFighterGameMode::FindMasterCamera()
{
	// 먼저 등록된 플레이어들에서 마스터 카메라 찾기
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player)
		{
			USmashCameraComponent* Camera = Player->FindComponentByClass<USmashCameraComponent>();
			if (Camera && Camera->IsMasterCamera())
			{
				return Camera;
			}
		}
	}

	// 마스터 카메라가 없으면 전체 캐릭터에서 검색
	for (TActorIterator<ASmashCharacter> It(GetWorld()); It; ++It)
	{
		ASmashCharacter* Character = *It;
		if (Character)
		{
			USmashCameraComponent* Camera = Character->FindComponentByClass<USmashCameraComponent>();
			if (Camera && Camera->IsMasterCamera())
			{
				return Camera;
			}
		}
	}

	return nullptr;
}

void ASmashPlatFighterGameMode::NotifyAllPlayersAboutPhaseChange(int32 NewPhase)
{
	// 모든 플레이어에게 페이즈 변경 알림
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player)
		{
			// 플레이어 컨트롤러 가져오기
			APlayerController* PC = Cast<APlayerController>(Player->GetController());
			if (PC)
			{
				// 클라이언트에 페이즈 변경 알림 전송
				// 여기서는 클라이언트 RPC를 통해 알릴 수 있습니다

				// 게임 인스턴스 업데이트 (필요한 경우)
				USmashGameInstance* GameInstance = Cast<USmashGameInstance>(GetGameInstance());
				if (GameInstance)
				{
					// 예: GameInstance->CurrentBossPhase = NewPhase;
				}

				// 효과음 재생 (모든 플레이어에게)
				if (PhaseChangeSound)
				{
					UGameplayStatics::PlaySound2D(this, PhaseChangeSound);
				}
			}
		}
	}
}

void ASmashPlatFighterGameMode::HandleBossPhaseChange(int32 NewPhase)
{
	// 모든 플레이어에게 페이즈 변경 알림
	NotifyAllPlayersAboutPhaseChange(NewPhase);

	// 모든 플레이어의 카메라에서 처리
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player && Player->SmashCameraComponent)
		{
			// 인터페이스 캐스팅 시도
			IInterface_BossEvents* BossEventInterface = Cast<IInterface_BossEvents>(Player->SmashCameraComponent);
			if (BossEventInterface)
			{
				// 인터페이스가 구현되어 있으면 메서드 호출
				BossEventInterface->HandleBossPhaseChange(NewPhase);
			}
		}
	}

	// 모든 플레이어의 카메라에 흔들림 효과 적용
	ShakeCamera(2.0f, 0.5f, 2.0f);
}

void ASmashPlatFighterGameMode::RegisterBoss(AActor* BossActor)
{
	// 모든 플레이어의 카메라에 보스 등록
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player && Player->SmashCameraComponent)
		{
			// 인터페이스 캐스팅 시도
			IInterface_BossEvents* BossEventInterface = Cast<IInterface_BossEvents>(Player->SmashCameraComponent);
			if (BossEventInterface)
			{
				// 인터페이스가 구현되어 있으면 메서드 호출
				BossEventInterface->RegisterBoss(BossActor);
			}
		}
	}
}

void ASmashPlatFighterGameMode::HandleGameOver(bool bVictory)
{
	// 모든 플레이어에게 게임 종료 알림
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player && Player->SmashCameraComponent)
		{
			// 인터페이스 캐스팅 시도
			IInterface_GameEvents* GameEventInterface = Cast<IInterface_GameEvents>(Player->SmashCameraComponent);
			if (GameEventInterface)
			{
				// 승리/패배에 따라 적절한 메서드 호출
				if (bVictory)
				{
					GameEventInterface->PlayVictorySequence();
				}
				else
				{
					GameEventInterface->PlayDefeatSequence();
				}
			}
		}
	}
}

void ASmashPlatFighterGameMode::ShakeCamera(float Intensity, float Duration, float Falloff)
{
	// 모든 플레이어의 카메라에 흔들림 효과 적용
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player && Player->SmashCameraComponent)
		{
			Player->SmashCameraComponent->ShakeCamera(Intensity, Duration, Falloff);
		}
	}
}

void ASmashPlatFighterGameMode::PlayBossIntroSequence(AActor* BossActor, float Duration)
{
	// 모든 플레이어의 카메라에서 보스 소개 시퀀스 재생
	for (ASmashCharacter* Player : RegisteredPlayers)
	{
		if (Player && Player->SmashCameraComponent)
		{
			// 인터페이스 캐스팅 시도
			IInterface_BossEvents* BossEventInterface = Cast<IInterface_BossEvents>(Player->SmashCameraComponent);
			if (BossEventInterface)
			{
				// 인터페이스가 구현되어 있으면 메서드 호출
				BossEventInterface->PlayBossIntroSequence(BossActor, Duration);
			}
		}
	}
}
