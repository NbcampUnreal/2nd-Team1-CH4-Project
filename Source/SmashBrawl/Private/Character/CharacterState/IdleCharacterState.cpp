// 


#include "Character/CharacterState/IdleCharacterState.h"


void UIdleCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	
	PlayerStateInfo.bCanMove = true;
	PlayerStateInfo.bCanFlip = true;
	PlayerStateInfo.bCanAttack = true;
	PlayerStateInfo.bCanJump = true;
	PlayerStateInfo.bCanSprint = true;
	PlayerStateInfo.JumpNumber = 0;

	//Capsual size변경 여기서?
}

UIdleCharacterState::UIdleCharacterState()
{
	Super::PlayerState = EPlayerStates::Idle;
}
