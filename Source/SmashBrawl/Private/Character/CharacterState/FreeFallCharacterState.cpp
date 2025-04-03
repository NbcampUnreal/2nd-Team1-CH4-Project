// 


#include "Character/CharacterState/FreeFallCharacterState.h"

void UFreeFallCharacterState::EnterState()
{
	Super::EnterState();
}

UFreeFallCharacterState::UFreeFallCharacterState()
{
	Super::PlayerState = EPlayerStates::FreeFall;
}
