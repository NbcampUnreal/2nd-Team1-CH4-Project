// 


#include "Character/CharacterState/HeldCharacterState.h"

void UHeldCharacterState::EnterState()
{
	Super::EnterState();
}

UHeldCharacterState::UHeldCharacterState()
{
	Super::PlayerState = EPlayerStates::Held;
}
