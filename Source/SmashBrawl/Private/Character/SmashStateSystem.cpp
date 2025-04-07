#include "Character/SmashStateSystem.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogSmashState);

USmashStateSystem::USmashStateSystem()
{
	// Tick을 활성화하여 상태 업데이트가 제대로 동작하도록 함
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 상태를 설정
	DefaultState = ESmashPlayerStates::Idle;
	CurrentStateID = DefaultState;
	bInitialized = false;
}

void USmashStateSystem::BeginPlay()
{
	Super::BeginPlay();

	// 초기 상태 생성 및 초기화
	for (TSubclassOf<UBaseCharacterState> InitState : InitStates)
	{
		if (!InitState)
			continue;

		if (UBaseCharacterState* NewState = NewObject<UBaseCharacterState>(GetOwner(), InitState))
		{
			NewState->InitState(this);
			States.Add(NewState);
		}
		else
		{
			UE_LOG(LogSmashState, Error, TEXT("UStateSystem :: 시작 플레이 - 상태 객체를 만들지 못했습니다"));
		}
	}

	// DefaultState가 InitStates에 포함되어 있는지 확인
	if (UBaseCharacterState* DefaultStateObj = FindState(DefaultState))
	{
		CurrentState = DefaultStateObj;
		CurrentStateID = DefaultState;
		CurrentState->EnterState();
	}
	else
	{
		UE_LOG(LogSmashState, Error, TEXT("UStateSystem::BeginPlay - 기본 상태를 찾을 수 없습니다. InitStates에 DefaultState를 추가하세요. 현재 기본 IDLE상태로 전이합니다."));
		if (UBaseCharacterState* IdleStateObj = FindState(ESmashPlayerStates::Idle))
		{
			CurrentState = IdleStateObj;
			CurrentStateID = ESmashPlayerStates::Idle;
			CurrentState->EnterState();
		}
	}

	bInitialized = true;
}

void USmashStateSystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 현재 상태 ID만 복제 (객체 참조 대신)
	DOREPLIFETIME(USmashStateSystem, CurrentStateID);
}

void USmashStateSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 초기화 및 null 체크 추가
	if (bInitialized && CurrentState)
	{
		CurrentState->TickState();
	}
}

UBaseCharacterState* USmashStateSystem::FindState(ESmashPlayerStates FindToState)
{
	for (UBaseCharacterState* State : States)
	{
		if (State && State->GetPlayerState() == FindToState)
		{
			return State;
		}
	}

	// 상태를 찾지 못한 경우 로그 출력
	UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::FindState - 상태를 찾을 수 없습니다: %s"), *UEnum::GetValueAsString(FindToState));
	return nullptr;
}

void USmashStateSystem::ChangeState(ESmashPlayerStates NewState)
{
	// 권한 체크 (서버권한이 아니라면)
	if (GetOwnerRole() < ROLE_Authority)
	{
		// 클라이언트에서 서버로 요청
		Server_ChangeState(NewState);
	}
	else
	{
		// 초기화 완료 체크
		if (!bInitialized)
		{
			UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::ChangeState - 상태 시스템이 초기화되지 않았습니다"));
			return;
		}

		// 같은 상태로 변경 시도하는 경우 처리
		if (CurrentStateID == NewState)
		{
			UE_LOG(LogSmashState, Verbose, TEXT("UStateSystem::ChangeState - 이미 상태에 있습니다: %s"), *UEnum::GetValueAsString(NewState));
			return;
		}

		UBaseCharacterState* NewChangeState = FindState(NewState);
		if (!NewChangeState)
		{
			UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::ChangeState - 상태를 찾을 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
			return;
		}

		// null 체크 추가
		if (CurrentState)
		{
			CurrentState->ExitState();
		}

		CurrentState = NewChangeState;
		CurrentStateID = NewState; // 복제되는 ID 업데이트
		CurrentState->EnterState();

		UE_LOG(LogSmashState, Log, TEXT("UStateSystem::ChangeState - 상태로 변경되었습니다: %s"), *UEnum::GetValueAsString(NewState));
	}
}

void USmashStateSystem::Server_ChangeState_Implementation(ESmashPlayerStates NewState)
{
	// 서버에서만 실행되는 함수
	ChangeState(NewState);
}

bool USmashStateSystem::TryChangeState(ESmashPlayerStates NewState, bool bCheckCanState)
{
	// 권한 체크 (서버권한이 아니라면)
	if (GetOwnerRole() < ROLE_Authority)
	{
		// 클라이언트에서 서버로 요청
		Server_TryChangeState(NewState, bCheckCanState);
		return true; // 클라이언트에서는 요청만 하고 항상 성공 반환
	}

	// 초기화 완료 체크
	if (!bInitialized)
	{
		UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::TryChangeState - 상태 시스템이 초기화되지 않았습니다"));
		return false;
	}

	// 같은 상태로 변경 시도하는 경우 성공으로 처리
	if (CurrentStateID == NewState)
	{
		UE_LOG(LogSmashState, Verbose, TEXT("UStateSystem::TryChangeState - 이미 상태에 있습니다: %s"), *UEnum::GetValueAsString(NewState));
		return true;
	}

	UBaseCharacterState* NewChangeState = FindState(NewState);
	if (!NewChangeState)
	{
		UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::TryChangeState - 상태를 찾을 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
		return false;
	}

	// bCheckCanState가 true일 때만 CanState 체크
	if (bCheckCanState)
	{
		// 상태 진입 가능 여부 확인
		if (NewChangeState->CanState())
		{
			// 조건 만족 시 상태 변경
			ChangeState(NewState);
			return true;
		}
		else
		{
			UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::TryChangeState - 상태로 변경할 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
			return false;
		}
	}
	else
	{
		// CanState 체크 없이 바로 상태 변경
		ChangeState(NewState);
		return true;
	}
}

void USmashStateSystem::Server_TryChangeState_Implementation(ESmashPlayerStates NewState, bool bCheckCanState)
{
	TryChangeState(NewState, bCheckCanState);
}

void USmashStateSystem::OnRep_CurrentStateID()
{
	// 초기화 체크
	if (!bInitialized)
	{
		UE_LOG(LogSmashState, Warning, TEXT("UStateSystem::OnRep_CurrentStateID - 상태 시스템이 초기화되지 않았습니다. ID: %s"), *UEnum::GetValueAsString(CurrentStateID));
		return;
	}

	// 복제된 상태 ID에 따라 CurrentState 업데이트
	UBaseCharacterState* NewState = FindState(CurrentStateID);

	// 상태를 찾지 못한 경우 복구 전략
	if (!NewState)
	{
		UE_LOG(LogSmashState, Error, TEXT("UStateSystem::OnRep_CurrentStateID - 상태를 찾을 수 없습니다: %s. DefaultState로 복구를 시도합니다."), *UEnum::GetValueAsString(CurrentStateID));

		// DefaultState로 복구 시도
		NewState = FindState(DefaultState);

		// 그래도 상태를 찾지 못한 경우
		if (!NewState)
		{
			UE_LOG(LogSmashState, Error, TEXT("UStateSystem::OnRep_CurrentStateID - DefaultState(%s)도 찾을 수 없습니다. 복구 불가능."), *UEnum::GetValueAsString(DefaultState));
			return;
		}

		// 복구한 상태 ID 설정
		CurrentStateID = DefaultState;
	}

	// 이전 상태 종료 및 새 상태 시작
	if (CurrentState && CurrentState != NewState)
	{
		CurrentState->ExitState();
	}

	CurrentState = NewState;
	CurrentState->EnterState();
}

ESmashPlayerStates USmashStateSystem::GetCurrentState() const
{
	return CurrentStateID;
}

void USmashStateSystem::DebugStateSystem()
{
	UE_LOG(LogSmashState, Log, TEXT("===== 상태 시스템 디버그 ====="));

	if (CurrentState)
	{
		UE_LOG(LogSmashState, Log, TEXT("현재 상태: %s"), *UEnum::GetValueAsString(CurrentState->GetPlayerState()));
	}
	else
	{
		UE_LOG(LogSmashState, Log, TEXT("현재 상태: None"));
	}

	UE_LOG(LogSmashState, Log, TEXT("사용 가능한 상태:"));
	for (UBaseCharacterState* State : States)
	{
		if (State)
		{
			UE_LOG(LogSmashState, Log, TEXT("  - %s"), *UEnum::GetValueAsString(State->GetPlayerState()));
		}
	}

	UE_LOG(LogSmashState, Log, TEXT("초기화 완료: %s"), bInitialized ? TEXT("예") : TEXT("아니오"));
	UE_LOG(LogSmashState, Log, TEXT("==========================="));
}
