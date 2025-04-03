// 


#include "Character/CharacterState/WalkOrRunCharacterState.h"

UWalkOrRunCharacterState::UWalkOrRunCharacterState()
{
	Super::PlayerState = EPlayerStates::WalkAndRun;
}

bool UWalkOrRunCharacterState::CanState()
{
	// return OwnerCharacter->bCanMove;
	return true;
}
