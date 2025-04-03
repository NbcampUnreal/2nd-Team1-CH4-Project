// 


#include "Character/CharacterState/ShieldCharacterState.h"

void UShieldCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
		
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = true;
}

UShieldCharacterState::UShieldCharacterState()
{
	Super::PlayerState = EPlayerStates::Shield;
}

