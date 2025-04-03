// 


#include "Character/CharacterState/FallCharacterState.h"

void UFallCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	
}

UFallCharacterState::UFallCharacterState()
{
	Super::PlayerState = EPlayerStates::Fall;
}
