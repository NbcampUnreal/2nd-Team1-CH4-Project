#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/Components/SmashStateSystem.h"
#include "Character/SmashCharacter.h"
#include "Character/Action/SmashAction.h"

UBaseCharacterState::UBaseCharacterState()
{
	PlayerState = ESmashPlayerStates::Idle;
    ElapsedTime = 0.0f; // 경과 시간 초기화
}

void UBaseCharacterState::InitState_Implementation(USmashStateSystem* InStateSystem)
{
	if (InStateSystem)
	{
		OwnerStateSystem = InStateSystem;
		OwnerCharacter = Cast<ASmashCharacter>(InStateSystem->GetOwner());
        
		// 이제 각 액션의 InitAction만 호출
		for (USmashAction* Action : Actions)
		{
			if (Action)
			{
				Action->InitAction(this);
			}
		}
	}
}

void UBaseCharacterState::EnterState_Implementation()
{
    // 상태 진입 시 경과 시간 초기화
    ElapsedTime = 0.0f;
    
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
        UE_LOG(LogSmashState, Verbose, TEXT("캐릭터 %s가 %s 상태를 종료했습니다 (지속 시간: %.2f초)"), 
            *OwnerCharacter->GetName(), *UEnum::GetValueAsString(PlayerState), ElapsedTime);
	}
}

void UBaseCharacterState::TickState_Implementation()
{
    // 상태 업데이트 로직 - 경과 시간 업데이트 추가
    if (OwnerStateSystem && OwnerStateSystem->GetWorld())
    {
        ElapsedTime += OwnerStateSystem->GetWorld()->GetDeltaSeconds();
    }
    
    // 자식 클래스에서 추가 구현 가능
}

bool UBaseCharacterState::CanState_Implementation()
{
	// 기본적으로 상태 전환 허용
	return true;
}

bool UBaseCharacterState::IsCurrentPlayerState(ESmashPlayerStates CheckState)
{
	return OwnerStateSystem->GetCurrentState() == CheckState;
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

USmashAction* UBaseCharacterState::FindActionByType(ESmashActionType ActionType, int Index)
{
	for (USmashAction* Action : Actions)
	{
		if (Index != 0)
		{
			return Actions[Index];
		}
		if (Action && Action->GetActionType() == ActionType)
		{
			return Action;
		}
	}

	return nullptr;
}

bool UBaseCharacterState::ExecuteActionByType(ESmashActionType ActionType, int Index)
{
	USmashAction* Action = FindActionByType(ActionType, Index);
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