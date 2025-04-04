#include "Character/StateSystem.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Net/UnrealNetwork.h"

UStateSystem::UStateSystem()
{
	// Tick을 활성화하여 상태 업데이트가 제대로 동작하도록 함
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 상태를 설정
	DefaultState = ESmashPlayerStates::Idle;
	CurrentStateID = DefaultState;
	bInitialized = false;
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

	// DefaultState가 InitStates에 포함되어 있는지 확인
	if (UBaseCharacterState* DefaultStateObj = FindState(DefaultState))
	{
		CurrentState = DefaultStateObj;
		CurrentStateID = DefaultState;
		CurrentState->EnterState();
		bInitialized = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UStateSystem::BeginPlay - 기본 상태를 찾을 수 없습니다. InitStates에 DefaultState를 추가하세요."));
	}
}

void UStateSystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 현재 상태 ID만 복제 (객체 참조 대신)
	DOREPLIFETIME(UStateSystem, CurrentStateID);
}

void UStateSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 초기화 및 null 체크 추가
	if (bInitialized && CurrentState)
	{
		CurrentState->TickState();
	}
}

UBaseCharacterState* UStateSystem::FindState(ESmashPlayerStates FindToState)
{
	for (UBaseCharacterState* State : States)
	{
		if (State && State->GetPlayerState() == FindToState)
		{
			return State;
		}
	}

	// 상태를 찾지 못한 경우 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("UStateSystem::FindState - 상태를 찾을 수 없습니다: %s"), *UEnum::GetValueAsString(FindToState));
	return nullptr;
}

void UStateSystem::ChangeState(ESmashPlayerStates NewState)
{
	// 권한 체크 추가 (서버권한이 아니라면)
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

void UStateSystem::Server_ChangeState_Implementation(ESmashPlayerStates NewState)
{
	// 서버에서만 실행되는 함수
	Internal_ChangeState(NewState);
}

void UStateSystem::Internal_ChangeState(ESmashPlayerStates NewState)
{
	// 초기화 완료 체크
	if (!bInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStateSystem::Internal_ChangeState - 상태 시스템이 초기화되지 않았습니다"));
		return;
	}

	// 같은 상태로 변경 시도하는 경우 처리
	if (CurrentStateID == NewState)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UStateSystem::Internal_ChangeState - 이미 상태에 있습니다: %s"), *UEnum::GetValueAsString(NewState));
		return;
	}

	UBaseCharacterState* NewChangeState = FindState(NewState);
	if (!NewChangeState)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStateSystem::Internal_ChangeState - 상태를 찾을 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
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
		CurrentStateID = NewState; // 복제되는 ID 업데이트
		CurrentState->EnterState();

		UE_LOG(LogTemp, Log, TEXT("UStateSystem::Internal_ChangeState - 상태로 변경되었습니다: %s"), *UEnum::GetValueAsString(NewState));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UStateSystem::Internal_ChangeState - 상태로 변경할 수 없습니다: %s"), *UEnum::GetValueAsString(NewState));
	}
}

void UStateSystem::OnRep_CurrentStateID()
{
	// 복제된 상태 ID에 따라 CurrentState 업데이트
	if (UBaseCharacterState* NewState = FindState(CurrentStateID))
	{
		// 이전 상태 종료
		if (CurrentState && CurrentState != NewState)
		{
			CurrentState->ExitState();
		}

		// 새 상태 설정 및 진입
		CurrentState = NewState;
		CurrentState->EnterState();
	}
}

ESmashPlayerStates UStateSystem::GetCurrentState() const
{
	return CurrentStateID;
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

	UE_LOG(LogTemp, Log, TEXT("초기화 완료: %s"), bInitialized ? TEXT("예") : TEXT("아니오"));
	UE_LOG(LogTemp, Log, TEXT("==========================="));
}
