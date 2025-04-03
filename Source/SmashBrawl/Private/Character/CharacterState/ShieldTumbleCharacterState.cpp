// 


#include "Character/CharacterState/ShieldTumbleCharacterState.h"

void UShieldTumbleCharacterState::EnterState()
{
	Super::EnterState();
	Super::PlayerState = EPlayerStates::Shield;
}
