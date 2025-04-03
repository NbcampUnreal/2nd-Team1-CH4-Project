// 


#include "Character/CharacterState/HoldCharacterState.h"

void UHoldCharacterState::EnterState()
{
	Super::EnterState();
}

UHoldCharacterState::UHoldCharacterState()
{
	Super::PlayerState = EPlayerStates::Hold;
}
