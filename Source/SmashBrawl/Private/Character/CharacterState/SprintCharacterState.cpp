// 


#include "Character/CharacterState/SprintCharacterState.h"

void USprintCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);

	PlayerStateInfo.bCanMove = true;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = true;
	PlayerStateInfo.bCanJump = true;
}

USprintCharacterState::USprintCharacterState()
{
	Super::PlayerState = EPlayerStates::Sprint;
}
