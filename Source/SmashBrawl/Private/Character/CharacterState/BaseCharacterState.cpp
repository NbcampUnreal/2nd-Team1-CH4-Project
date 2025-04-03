// 


#include "Character/CharacterState/BaseCharacterState.h"

#include "Net/UnrealNetwork.h"

UBaseCharacterState::UBaseCharacterState()
{
	PlayerState = EPlayerStates::Idle;
	PlayerStateInfo = FCharacterStateInfo();
}

EPlayerStates UBaseCharacterState::GetPlayerState() const
{
	return PlayerState;
}

void UBaseCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	if (BeforeCharacterState)
	{
		PlayerStateInfo = BeforeCharacterState->GetStateInfo();
	}
}

void UBaseCharacterState::TickState()
{
}

void UBaseCharacterState::ExitState()
{
}

FCharacterStateInfo UBaseCharacterState::GetStateInfo()
{
	return PlayerStateInfo;
}

bool UBaseCharacterState::CanState()
{
	return true;
}

void UBaseCharacterState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseCharacterState, PlayerStateInfo.bCanAttack);
}
