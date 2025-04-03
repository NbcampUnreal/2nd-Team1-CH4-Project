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

bool UWalkOrRunCharacterState::CanState()
{
	// return OwnerCharacter->bCanMove;
	return true;
}
