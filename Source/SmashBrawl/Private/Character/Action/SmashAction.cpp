#include "Character/Action/SmashAction.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/SmashCharacter.h"
#include "Character/SmashStateSystem.h"

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
	return AreStateConditionsMet();
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

bool USmashAction::AreStateConditionsMet() const
{
	// 상태 참조 또는 필요 상태 목록이 없으면 항상 허용
	if (!OwnerState || RequiredStates.Num() == 0)
		return true;

	// 상태 시스템 참조 체크
	if (!OwnerState->OwnerStateSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("USmashAction::AreStateConditionsMet - 상태 시스템이 유효하지 않습니다."));
		return false;
	}

	// 현재 캐릭터 상태가 필요 상태 목록에 포함되어 있는지 확인
	ESmashPlayerStates CurrentState = OwnerState->OwnerStateSystem->GetCurrentState();
	return RequiredStates.Contains(CurrentState);
}
