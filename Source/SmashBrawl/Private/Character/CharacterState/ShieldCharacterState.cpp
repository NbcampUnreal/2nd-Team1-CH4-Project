// 


#include "Character/CharacterState/ShieldCharacterState.h"

void UShieldCharacterState::EnterState()
{
	Super::EnterState();
}

UShieldCharacterState::UShieldCharacterState()
{
	Super::PlayerState = EPlayerStates::Shield;
}

