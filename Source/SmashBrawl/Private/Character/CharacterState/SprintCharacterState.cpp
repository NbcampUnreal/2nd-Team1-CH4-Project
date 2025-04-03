// 


#include "Character/CharacterState/SprintCharacterState.h"

void USprintCharacterState::EnterState()
{
	Super::EnterState();
}

USprintCharacterState::USprintCharacterState()
{
	Super::PlayerState = EPlayerStates::Sprint;
}
