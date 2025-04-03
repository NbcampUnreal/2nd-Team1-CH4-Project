// 


#include "Character/CharacterState/WalkOrRunCharacterState.h"

void UWalkOrRunCharacterState::EnterState()
{
	Super::EnterState();
}

UWalkOrRunCharacterState::UWalkOrRunCharacterState()
{
	Super::PlayerState = EPlayerStates::WalkAndRun;
}
