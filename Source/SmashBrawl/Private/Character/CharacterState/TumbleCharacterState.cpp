// 


#include "Character/CharacterState/TumbleCharacterState.h"

void UTumbleCharacterState::EnterState()
{
	Super::EnterState();
}

UTumbleCharacterState::UTumbleCharacterState()
{
	Super::PlayerState = EPlayerStates::Tumble;
}
