// 


#include "Character/CharacterState/HitCharacterState.h"

void UHitCharacterState::EnterState()
{
	Super::EnterState();
}

UHitCharacterState::UHitCharacterState()
{
	Super::PlayerState = EPlayerStates::Hit;
}
