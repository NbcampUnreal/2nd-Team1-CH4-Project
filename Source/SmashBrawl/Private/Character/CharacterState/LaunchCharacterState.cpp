// 


#include "Character/CharacterState/LaunchCharacterState.h"

void ULaunchCharacterState::EnterState()
{
	Super::EnterState();
}

ULaunchCharacterState::ULaunchCharacterState()
{
	Super::PlayerState = EPlayerStates::Launch;
}
