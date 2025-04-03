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


void UIdleCharacterState::EnterState()
{
	// OwnerCharacter->JumpNumber = 0;
	// FuncCapusleSize();
	// FuncSetMovementStates();
	// OwnerCharacter->bCanSprint = true;
	// PlatformDrop();
	// if (OwnerCharacter->UpDown <= -0.3)
	// {
	// 	FuncSetState(EPlayerStates::Crouch);
	// 	DoExecute= false;
	// }
	// else
	// {
	// 	if (!DoExecute)
	// 	{
	// 		FuncSetState(EPlayerStates::Idle);
	// 		DoExecute = true;
	// 	}
	// }	
}

void UIdleCharacterState::ExitState()
{
	
}

bool UIdleCharacterState::CanState()
{
	return true;
}

