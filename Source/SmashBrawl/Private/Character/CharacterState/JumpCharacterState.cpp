// 


#include "Character/CharacterState/JumpCharacterState.h"

void UJumpCharacterState::EnterState()
{
	Super::EnterState();
}

UJumpCharacterState::UJumpCharacterState()
{
	Super::PlayerState = EPlayerStates::Jump;
}
