// 


#include "Character/CharacterState/LaunchCharacterState.h"

void ULaunchCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);
	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;

	//CapsualSize 변경 및 날라가는 정도 넣어둘것
}

ULaunchCharacterState::ULaunchCharacterState()
{
	Super::PlayerState = EPlayerStates::Launch;
}
