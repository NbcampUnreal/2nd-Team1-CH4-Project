#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/StateSystem.h"
#include "Character/BaseSSTCharacter.h"
#include "Net/UnrealNetwork.h"

UBaseCharacterState::UBaseCharacterState()
{
	PlayerState = EPlayerStates::Idle;
}

void UBaseCharacterState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 상태 정보를 네트워크로 복제
	DOREPLIFETIME(UBaseCharacterState, StateInfo);
}

void UBaseCharacterState::InitState_Implementation(UStateSystem* InStateSystem)
{
	if (InStateSystem)
	{
		OwnerStateSystem = InStateSystem;
		OwnerCharacter = Cast<ABaseSSTCharacter>(InStateSystem->GetOwner());
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

void UBaseCharacterState::OnRep_StateInfo()
{
	// 상태 정보가 복제되면 실행할 로직
	// 자식 클래스에서 필요시 구현
}

void UBaseCharacterState::SetStateInfo(const FPlayerStateInfo& NewStateInfo)
{
	// 서버에서만 상태 정보 변경
	if (OwnerCharacter && OwnerCharacter->GetLocalRole() == ROLE_Authority)
	{
		StateInfo = NewStateInfo;
		OnRep_StateInfo();
	}
}

EPlayerStates UBaseCharacterState::GetPlayerState() const
{
	return PlayerState;
}
