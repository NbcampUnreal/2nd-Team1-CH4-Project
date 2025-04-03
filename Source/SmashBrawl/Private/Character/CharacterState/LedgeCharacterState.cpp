// 


#include "Character/CharacterState/LedgeCharacterState.h"

void ULedgeCharacterState::EnterState()
{
	Super::EnterState();
}

ULedgeCharacterState::ULedgeCharacterState()
{
	Super::PlayerState = EPlayerStates::Ledge;
}
