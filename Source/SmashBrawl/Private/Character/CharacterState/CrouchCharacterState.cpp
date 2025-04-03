// 


#include "Character/CharacterState/CrouchCharacterState.h"

void UCrouchCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);

	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
}

UCrouchCharacterState::UCrouchCharacterState()
{
	Super::PlayerState = EPlayerStates::Crouch;
}
