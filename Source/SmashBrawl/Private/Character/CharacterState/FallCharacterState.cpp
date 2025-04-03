// 


#include "Character/CharacterState/FallCharacterState.h"

void UFallCharacterState::EnterState()
{
	Super::EnterState();
}

UFallCharacterState::UFallCharacterState()
{
	Super::PlayerState = EPlayerStates::Fall;
}
