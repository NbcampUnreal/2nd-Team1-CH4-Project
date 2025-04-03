// 


#include "Character/CharacterState/DizzyCharacterState.h"

void UDizzyCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

UDizzyCharacterState::UDizzyCharacterState()
{
	Super::PlayerState = EPlayerStates::Dizzy;
}
