// 


#include "Character/CharacterState/BaseCharacterState.h"

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
