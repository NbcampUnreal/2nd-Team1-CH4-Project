// 


#include "Character/CharacterState/HeldCharacterState.h"

void UHeldCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

UHeldCharacterState::UHeldCharacterState()
{
	Super::PlayerState = EPlayerStates::Held;
}
