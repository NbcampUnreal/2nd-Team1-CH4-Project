#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/StateSystem.h"
#include "Character/SmashCharacter.h"

UBaseCharacterState::UBaseCharacterState()
{
	PlayerState = ESmashPlayerStates::Idle;
}

void UBaseCharacterState::InitState_Implementation(UStateSystem* InStateSystem)
{
	if (InStateSystem)
	{
		OwnerStateSystem = InStateSystem;
		OwnerCharacter = Cast<ASmashCharacter>(InStateSystem->GetOwner());
	}
}

void UBaseCharacterState::EnterState_Implementation()
{
	// 상태 진입 시 로직
	if (OwnerCharacter)
	{
		UE_LOG(LogTemp, Verbose, TEXT("캐릭터 %s가 %s 상태에 진입했습니다"), *OwnerCharacter->GetName(), *UEnum::GetValueAsString(PlayerState));
	}
}

void UBaseCharacterState::ExitState_Implementation()
{
	// 상태 종료 시 로직
	if (OwnerCharacter)
	{
		UE_LOG(LogTemp, Verbose, TEXT("캐릭터 %s가 %s 상태를 종료했습니다"), *OwnerCharacter->GetName(), *UEnum::GetValueAsString(PlayerState));
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
	if (OwnerCharacter && OwnerCharacter->GetLocalRole() == ROLE_Authority)
	{
		OwnerCharacter->SetStateInfo(NewStateInfo);
	}
	else if (OwnerCharacter)
	{
		// 클라이언트에서는 서버에 요청하는 로직이 필요함
		// 이는 SmashCharacter에서 구현할 예정
	}
}

ESmashPlayerStates UBaseCharacterState::GetPlayerState() const
{
	return PlayerState;
}
