#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/SmashStateSystem.h"
#include "Character/SmashCharacter.h"
#include "Character/Action/SmashAction.h"

UBaseCharacterState::UBaseCharacterState()
{
	PlayerState = ESmashPlayerStates::Idle;
}

void UBaseCharacterState::InitState_Implementation(USmashStateSystem* InStateSystem)
{
	if (InStateSystem)
	{
		OwnerStateSystem = InStateSystem;
		OwnerCharacter = Cast<ASmashCharacter>(InStateSystem->GetOwner());

		// 액션 초기화
		InitActions();
	}
}

void UBaseCharacterState::EnterState_Implementation()
{
	// 상태 진입 시 로직
	if (OwnerCharacter)
	{
		UE_LOG(LogSmashState, Verbose, TEXT("캐릭터 %s가 %s 상태에 진입했습니다"), *OwnerCharacter->GetName(), *UEnum::GetValueAsString(PlayerState));
	}
}

void UBaseCharacterState::ExitState_Implementation()
{
	// 상태 종료 시 로직
	if (OwnerCharacter)
	{
		UE_LOG(LogSmashState, Verbose, TEXT("캐릭터 %s가 %s 상태를 종료했습니다"), *OwnerCharacter->GetName(), *UEnum::GetValueAsString(PlayerState));
	}
}

void UBaseCharacterState::TickState_Implementation()
{
	// 상태 업데이트 로직
	// 자식 클래스에서 구현
}

bool UBaseCharacterState::CanState_Implementation()
{
	// 기본적으로 상태 전환 허용
	return true;
}

void UBaseCharacterState::InitActions()
{
	// 기존 액션 제거
	Actions.Empty();

	// 액션 클래스를 기반으로 액션 생성
	for (TSubclassOf<USmashAction> ActionClass : ActionClasses)
	{
		if (ActionClass)
		{
			USmashAction* NewAction = NewObject<USmashAction>(this, ActionClass);
			if (NewAction)
			{
				NewAction->InitAction(this);
				Actions.Add(NewAction);
			}
		}
	}
}

const FSmashPlayerStateInfo& UBaseCharacterState::GetStateInfo() const
{
	// 캐릭터의 상태 정보에 접근
	if (OwnerCharacter)
	{
		return OwnerCharacter->GetStateInfo();
	}

	// 캐릭터가 없는 경우 기본값 반환
	static const FSmashPlayerStateInfo DefaultStateInfo;
	return DefaultStateInfo;
}

void UBaseCharacterState::SetStateInfo(const FSmashPlayerStateInfo& NewStateInfo)
{
	// 캐릭터의 상태 정보 설정 - 서버 권한 확인 추가
	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetLocalRole() == ROLE_Authority)
		{
			OwnerCharacter->SetStateInfo(NewStateInfo);
		}
		else
		{
			// 클라이언트에서는 서버에 요청
			OwnerCharacter->Server_RequestSetStateInfo(NewStateInfo);
		}
	}
	else
	{
		UE_LOG(LogSmashState, Error, TEXT("UBaseCharacterState::SetStateInfo - OwnerCharacter가 유효하지 않습니다."));
	}
}



USmashAction* UBaseCharacterState::FindActionByType(ESmashActionType ActionType)
{
	for (USmashAction* Action : Actions)
	{
		if (Action && Action->GetActionType() == ActionType)
		{
			return Action;
		}
	}

	return nullptr;
}

bool UBaseCharacterState::ExecuteActionByType(ESmashActionType ActionType)
{
	USmashAction* Action = FindActionByType(ActionType);
	if (Action && Action->CanExecute())
	{
		return Action->Execute();
	}

	return false;
}

ESmashPlayerStates UBaseCharacterState::GetPlayerState() const
{
	return PlayerState;
}
