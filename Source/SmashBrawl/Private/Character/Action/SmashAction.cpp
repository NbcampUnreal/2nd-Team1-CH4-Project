#include "Character/Action/SmashAction.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/SmashCharacter.h"
#include "Character/Components/SmashStateSystem.h"

USmashAction::USmashAction(): ActionType()
{
}

void USmashAction::InitAction_Implementation(UBaseCharacterState* InOwnerState)
{
	OwnerState = InOwnerState;
}

bool USmashAction::Execute_Implementation()
{
	// 기본 구현은 빈 로직, 자식 클래스에서 오버라이드
	return true;
}

bool USmashAction::CanExecute_Implementation()
{
	// 상태 조건 검사를 포함한 실행 가능 여부 확인
	return true;
}

ASmashCharacter* USmashAction::GetOwnerCharacter() const
{
	if (OwnerState)
	{
		// 수정: GetOuter() 대신 OwnerCharacter 직접 참조
		return OwnerState->OwnerCharacter;
	}
	return nullptr;
}
