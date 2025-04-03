#include "Character/StateSystem.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Net/UnrealNetwork.h"

UStateSystem::UStateSystem()
{
	// Tick을 활성화하여 상태 업데이트가 제대로 동작하도록 함
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 상태를 설정
	DefaultState = EPlayerStates::Idle;
}

void UStateSystem::BeginPlay()
{
	Super::BeginPlay();

	// 초기 상태 생성 및 초기화
	for (TSubclassOf<UBaseCharacterState> InitState : InitStates)
	{
		if (InitState)
		{
			if (UBaseCharacterState* NewState = NewObject<UBaseCharacterState>(GetOwner(), InitState))
			{
				NewState->InitState(this);
				States.Add(NewState);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UStateSystem :: 시작 플레이 - 상태 객체를 만들지 못했습니다"));
			}
		}
	}

	// 기본 상태 설정 및 null 체크 추가
	if (UBaseCharacterState* DefaultStateObj = FindState(DefaultState))
	{
		CurrentState = DefaultStateObj;
		CurrentState->EnterState();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UStateSystem::BeginPlay - 기본 상태를 찾을 수 없습니다"));
	}
}

void UStateSystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 상태 시스템 속성들을 네트워크로 복제하도록 설정
	DOREPLIFETIME(UStateSystem, States);
	DOREPLIFETIME(UStateSystem, CurrentState);
}

void UStateSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// null 체크 추가
	if (CurrentState)
	{
		CurrentState->TickState();
	}
}

UBaseCharacterState* UStateSystem::FindState(EPlayerStates FindToState)
{
	for (UBaseCharacterState* State : States)
	{
		if (State && State->GetPlayerState() == FindToState)
		{
			return State;
		}
	}
	return nullptr;
}

void UStateSystem::ChangeState(EPlayerStates NewState)
{
	// 권한 체크 추가
	if (GetOwnerRole() < ROLE_Authority)
	{
		// 클라이언트에서 서버로 요청
		Server_ChangeState(NewState);
	}
	else
	{
		// 서버에서 직접 실행
		Internal_ChangeState(NewState);
	}
}

void UStateSystem::Server_ChangeState_Implementation(EPlayerStates NewState)
{
	// 서버에서만 실행되는 함수
	Internal_ChangeState(NewState);
}

void UStateSystem::Internal_ChangeState(EPlayerStates NewState)
{
	UBaseCharacterState* NewChangeState = FindState(NewState);
	if (!NewChangeState)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStateSystem::Internal_ChangeState - 상태를 찾을 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
		return;
	}

	// 같은 상태로 변경 시도하는 경우 처리
	if (CurrentState == NewChangeState)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UStateSystem::Internal_ChangeState - 이미 상태에 있습니다: %s"),
		       *UEnum::GetValueAsString(NewState));
		return;
	}

	if (NewChangeState->CanState())
	{
		// null 체크 추가
		if (CurrentState)
		{
			CurrentState->ExitState();
		}

		CurrentState = NewChangeState;
		CurrentState->EnterState();

		UE_LOG(LogTemp, Log, TEXT("UStateSystem::Internal_ChangeState - 상태로 변경되었습니다: %s"), *UEnum::GetValueAsString(NewState));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UStateSystem::Internal_ChangeState - 상태로 변경할 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
	}
}

EPlayerStates UStateSystem::GetCurrentState() const
{
	// null 체크 추가
	if (CurrentState)
	{
		return CurrentState->GetPlayerState();
	}
	return DefaultState;
}

void UStateSystem::DebugStateSystem()
{
	UE_LOG(LogTemp, Log, TEXT("===== 상태 시스템 디버그 ====="));

	if (CurrentState)
	{
		UE_LOG(LogTemp, Log, TEXT("현재 상태: %s"), *UEnum::GetValueAsString(CurrentState->GetPlayerState()));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("현재 상태: None"));
	}

	UE_LOG(LogTemp, Log, TEXT("사용 가능한 상태:"));
	for (UBaseCharacterState* State : States)
	{
		if (State)
		{
			UE_LOG(LogTemp, Log, TEXT("  - %s"), *UEnum::GetValueAsString(State->GetPlayerState()));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("==========================="));
}
