// 


#include "Character/CharacterState/TumbleCharacterState.h"

void UTumbleCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = true;
	PlayerStateInfo.bCanAttack = true;
	PlayerStateInfo.bCanJump = false;
}

UTumbleCharacterState::UTumbleCharacterState()
{
	Super::PlayerState = EPlayerStates::Tumble;
}
