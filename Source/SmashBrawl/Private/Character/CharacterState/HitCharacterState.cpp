// 


#include "Character/CharacterState/HitCharacterState.h"

void UHitCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	Super::EnterState(BeforeCharacterState);

	PlayerStateInfo.bCanMove = false;
	PlayerStateInfo.bCanFlip = false;
	PlayerStateInfo.bCanAttack = false;
	PlayerStateInfo.bCanJump = false;
}

UHitCharacterState::UHitCharacterState()
{
	Super::PlayerState = EPlayerStates::Hit;
}
