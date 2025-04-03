// 


#include "Character/CharacterState/ProneCharacterState.h"

void UProneCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

UProneCharacterState::UProneCharacterState()
{
	Super::PlayerState = EPlayerStates::Prone;
}
