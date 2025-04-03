// 


#include "Character/CharacterState/IdleCharacterState.h"

#include "Character/BaseSSTCharacter.h"

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

