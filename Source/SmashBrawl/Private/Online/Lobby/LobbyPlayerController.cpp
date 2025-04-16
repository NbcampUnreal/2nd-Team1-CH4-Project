// 


#include "Online/Lobby/LobbyPlayerController.h"

#include "Online/Lobby/LobbyMode.h"
#include "Online/Lobby/LobbyPlayerState.h"

ALobbyPlayerController::ALobbyPlayerController()
{
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(true);
	
	if (ALobbyPlayerState* LPlayerState = GetPlayerState<ALobbyPlayerState>())
	{
		LobbyPlayerState = LPlayerState;
	}
}

void ALobbyPlayerController::ServerDebugPlayerStart_Implementation()
{
	if (ALobbyMode* GameMode = Cast<ALobbyMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->StartMatch();
	}
}

void ALobbyPlayerController::ServerPlayerReady_Implementation()
{
	if (ALobbyMode* GameMode = Cast<ALobbyMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerReady(this);
	}
	if (LobbyPlayerState)
	{

		LobbyPlayerState->bReady = true;
	}
}

void ALobbyPlayerController::ServerPlayerUnReady_Implementation()
{
	if (ALobbyMode* GameMode = Cast<ALobbyMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerUnReady(this);
	}
	
	if (LobbyPlayerState)
	{
		LobbyPlayerState->bReady = false;
	}
}
