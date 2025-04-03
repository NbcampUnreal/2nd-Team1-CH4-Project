// 


#include "Character/CharacterState/SprintCharacterState.h"

void USprintCharacterState::EnterState(IInterface_CharacterState* BeforeCharacterState)
{
	//나 속도가 거의 정지상태면 IDLE로 갈거야.

	//1.상태자체로직안에서 다른 상태로 가는 거는 앵간하면 지양해라.
	//2.
}

void USprintCharacterState::TickState()
{
	
}

USprintCharacterState::USprintCharacterState()
{
	Super::PlayerState = EPlayerStates::Sprint;
}
