// 


#include "Character/CharacterState/IdleCharacterState.h"


void UIdleCharacterState::EnterState()
{
	Super::EnterState();
}

UIdleCharacterState::UIdleCharacterState()
{
	Super::PlayerState = EPlayerStates::Idle;
}
