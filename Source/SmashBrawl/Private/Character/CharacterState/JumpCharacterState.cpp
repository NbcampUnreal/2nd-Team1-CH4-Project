// 


#include "Character/CharacterState/JumpCharacterState.h"

void UJumpCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);

	PlayerStateInfo.bCanMove = true;
	PlayerStateInfo.bCanFlip = false;
	
}

UJumpCharacterState::UJumpCharacterState()
{
	Super::PlayerState = EPlayerStates::Jump;
}

