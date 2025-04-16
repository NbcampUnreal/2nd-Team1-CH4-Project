// 


#include "Online/Lobby/LobbyPlayerState.h"

#include "Net/UnrealNetwork.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bReady);
}

void ALobbyPlayerState::ChangeReadyState()
{
	bReady = !bReady;
}

bool ALobbyPlayerState::GetIsReady() const
{
	return bReady;
}
