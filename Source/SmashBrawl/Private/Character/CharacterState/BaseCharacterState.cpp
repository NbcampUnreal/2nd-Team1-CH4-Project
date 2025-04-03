// 


#include "Character/CharacterState/BaseCharacterState.h"

UBaseCharacterState::UBaseCharacterState() : PlayerState(EPlayerStates::Idle)
{
	
}

EPlayerStates UBaseCharacterState::GetPlayerState() const
{
	return PlayerState;
}

void UBaseCharacterState::EnterState()
{
}

void UBaseCharacterState::TickState()
{
}

void UBaseCharacterState::ExitState()
{
}
