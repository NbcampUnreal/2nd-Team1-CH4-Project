// 


#include "Character/CharacterState/ProneCharacterState.h"

void UProneCharacterState::EnterState()
{
	Super::EnterState();
}

UProneCharacterState::UProneCharacterState()
{
	Super::PlayerState = EPlayerStates::Prone;
}
