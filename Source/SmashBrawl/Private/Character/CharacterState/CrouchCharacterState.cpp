// 


#include "Character/CharacterState/CrouchCharacterState.h"

void UCrouchCharacterState::EnterState()
{
	Super::EnterState();
	
}

UCrouchCharacterState::UCrouchCharacterState()
{
	Super::PlayerState = EPlayerStates::Crouch;
}
