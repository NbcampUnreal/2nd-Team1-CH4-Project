// 


#include "Online/Lobby/LobbyMode.h"

#include "Core/SmashPlayerController.h"
#include "Online/Lobby/LobbyGameState.h"

ALobbyMode::ALobbyMode()
{
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerControllerClass = ASmashPlayerController::StaticClass();
	bDelayedStart = true;
}

void ALobbyMode::BeginPlay()
{
	Super::BeginPlay();

	LobbyGameState = GetGameState<ALobbyGameState>();
}

void ALobbyMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (LobbyGameState)
	{
		LobbyGameState->AddJoinPlayer(NewPlayer);
	}
}

void ALobbyMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (LobbyGameState)
	{
		if (APlayerController* Controller = Cast<APlayerController>(Exiting))
		{
			LobbyGameState->RemoveReadPlayer(Controller);
			LobbyGameState->RemoveJoinPlayer(Controller);
		}
	}
}

void ALobbyMode::StartMatch()
{
	Super::StartMatch();

	OnMatchStart.Broadcast();
	// 플레이어 정보를 GameInstance에 담을것!
	GetWorld()->GetTimerManager().SetTimer(PlayTimerHandle, this, &ALobbyMode::ServerTravelLevel, 0.1f, false, 3.0f);
}

void ALobbyMode::ServerTravelLevel()
{
	GetWorld()->ServerTravel(PlayMapPath +"?listen");
}

void ALobbyMode::PlayerUnReady(APlayerController* NewPlayer)
{
	if (LobbyGameState &&
		!LobbyGameState->HasMatchStarted())
	{
		LobbyGameState->RemoveReadPlayer(NewPlayer);
	}
}

bool ALobbyMode::bCanJoinPlayer()
{
	if (LobbyGameState &&
		!LobbyGameState->HasMatchStarted() &&
		LobbyGameState->GetNumJoinPlayers() < 4)
	{
		return true;
	}
	return false;
}

void ALobbyMode::PlayerReady(APlayerController* NewPlayer)
{
	if (LobbyGameState)
	{
		LobbyGameState->AddReadPlayer(NewPlayer);

		if (LobbyGameState->GetNumJoinPlayers() >= 1 &&
			LobbyGameState->GetNumJoinPlayers() == LobbyGameState->GetNumReadPlayers())
		{
			UE_LOG(LogTemp,Warning,TEXT("2ALobbyPlayerController::ServerPlayerReady_Implementation"));

			StartMatch();
		}
	}

}
