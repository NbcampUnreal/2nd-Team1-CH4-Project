// 


#include "Character/CharacterState/HoldCharacterState.h"

void UHoldCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

UHoldCharacterState::UHoldCharacterState()
{
	Super::PlayerState = EPlayerStates::Hold;
}
