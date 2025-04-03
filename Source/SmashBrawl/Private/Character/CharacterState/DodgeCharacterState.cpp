// 


#include "Character/CharacterState/DodgeCharacterState.h"

void UDodgeCharacterState::EnterState()
{
	Super::EnterState();
}

UDodgeCharacterState::UDodgeCharacterState()
{
	Super::PlayerState = EPlayerStates::Dodge;
}
