// 


#include "Character/CharacterState/JumpCharacterState.h"


UJumpCharacterState::UJumpCharacterState()
{
	Super::PlayerState = EPlayerStates::Jump;
}


void UJumpCharacterState::EnterState()
{
	Super::EnterState();
}
