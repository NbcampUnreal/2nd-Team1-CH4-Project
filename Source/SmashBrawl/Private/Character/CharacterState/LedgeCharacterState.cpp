// 


#include "Character/CharacterState/LedgeCharacterState.h"

void ULedgeCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);

	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

ULedgeCharacterState::ULedgeCharacterState()
{
	Super::PlayerState = EPlayerStates::Ledge;
}
