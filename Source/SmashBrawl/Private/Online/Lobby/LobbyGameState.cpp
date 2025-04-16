// 


#include "Online/Lobby/LobbyGameState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameState::AddJoinPlayer(const APlayerController* NewPlayer)
{
	JoinPlayers.Add(NewPlayer->GetPlayerState<APlayerState>());
}

void ALobbyGameState::RemoveJoinPlayer(const APlayerController* NewPlayer)
{
	JoinPlayers.Remove(NewPlayer->GetPlayerState<APlayerState>());
}

int32 ALobbyGameState::GetNumJoinPlayers() const
{
	return JoinPlayers.Num();
}

void ALobbyGameState::AddReadPlayer(const APlayerController* NewPlayer)
{
	ReadyPlayers.Add(NewPlayer->GetPlayerState<APlayerState>());
}

void ALobbyGameState::RemoveReadPlayer(const APlayerController* NewPlayer)
{
	ReadyPlayers.Remove(NewPlayer->GetPlayerState<APlayerState>());
}

int32 ALobbyGameState::GetNumReadPlayers() const
{
	return ReadyPlayers.Num();
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, JoinPlayers);
	DOREPLIFETIME(ALobbyGameState, ReadyPlayers);
}
