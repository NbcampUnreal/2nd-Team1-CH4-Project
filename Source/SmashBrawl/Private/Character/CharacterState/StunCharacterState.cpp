// 


#include "Character/CharacterState/StunCharacterState.h"

void UStunCharacterState::EnterState()
{
	Super::EnterState();
}

UStunCharacterState::UStunCharacterState()
{
	Super::PlayerState = EPlayerStates::Stun;
}
