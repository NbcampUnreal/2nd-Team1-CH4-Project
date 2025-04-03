// 


#include "Character/CharacterState/DeadCharacterState.h"

void UDeadCharacterState::EnterState()
{
	Super::EnterState();
	
}

UDeadCharacterState::UDeadCharacterState()
{
	Super::PlayerState = EPlayerStates::Dead;
}
