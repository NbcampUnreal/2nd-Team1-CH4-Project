// 


#include "Character/CharacterState/FreeFallCharacterState.h"

void UFreeFallCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	PlayerStateInfo.bCanMove = true;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

UFreeFallCharacterState::UFreeFallCharacterState()
{
	Super::PlayerState = EPlayerStates::FreeFall;
}
