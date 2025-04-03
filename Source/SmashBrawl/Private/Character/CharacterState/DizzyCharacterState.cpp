// 


#include "Character/CharacterState/DizzyCharacterState.h"

void UDizzyCharacterState::EnterState()
{
	Super::EnterState();
	
}

UDizzyCharacterState::UDizzyCharacterState()
{
	Super::PlayerState = EPlayerStates::Dizzy;
}
