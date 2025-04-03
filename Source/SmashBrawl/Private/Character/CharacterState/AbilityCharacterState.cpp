// 


#include "Character/CharacterState/AbilityCharacterState.h"

UAbilityCharacterState::UAbilityCharacterState()
{
	Super::PlayerState = EPlayerStates::Ability;
}

void UAbilityCharacterState::EnterState()
{
	Super::EnterState();
}
