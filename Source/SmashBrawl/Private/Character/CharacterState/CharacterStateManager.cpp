// 


#include "Character/CharacterState/CharacterStateManager.h"

#include "Character/BaseSSTCharacter.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/CharacterState/IdleCharacterState.h"
#include "Character/CharacterState/JumpCharacterState.h"
#include "Character/CharacterState/WalkOrRunCharacterState.h"
#include "Net/UnrealNetwork.h"


UCharacterStateManager::UCharacterStateManager()
{
	CharacterStates.Add(UIdleCharacterState::StaticClass());
	CharacterStates.Add(UJumpCharacterState::StaticClass());
	CharacterStates.Add(UWalkOrRunCharacterState::StaticClass());
}

void UCharacterStateManager::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UBaseCharacterState> Class : CharacterStates)
	{
		if (TObjectPtr<UBaseCharacterState> NewState = NewObject<UBaseCharacterState>(this, Class))
		{
			NewState->OwnerCharacter = Cast<ABaseSSTCharacter>(GetOwner());
			CharacterStatePtrs.Add(NewState);
		}
	}

	//맨 처음 초기상태는 IDLE 로 지정
	CurrentState = FindState(EPlayerStates::Idle);
}

void UCharacterStateManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	CurrentState->TickState();
}

void UCharacterStateManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterStateManager, CurrentState);
}

void UCharacterStateManager::ChangeCharacterState(EPlayerStates NewState)
{
	UBaseCharacterState* NextState = FindState(NewState);
	if (!NextState)
		return;

	//다음 상태로 변경을 허용한다면
	if (NextState->CanState())
	{
		CurrentState->ExitState();
		CurrentState = NextState;
		CurrentState->EnterState();
	}
	else //다음 상태로 변경을 허용하지 않는다면 기본 IDLE상태로 유지합니다.
	{
		CurrentState = FindState(EPlayerStates::Idle);
	}
}

EPlayerStates UCharacterStateManager::GetCurrentState() const
{
	if (CurrentState)
	{
		return CurrentState->GetPlayerState();
	}
	return EPlayerStates::Idle;
}

bool UCharacterStateManager::bCanAttack() const
{
	if (CurrentState->GetPlayerState() != EPlayerStates::Ability)
	{
		return true;
	}
	return false;
}

void UCharacterStateManager::InitializeComponent()
{
	Super::InitializeComponent();
}

UBaseCharacterState* UCharacterStateManager::FindState(EPlayerStates FidState) const
{
	for (auto BaseCharacterState : CharacterStatePtrs)
	{
		if (BaseCharacterState->PlayerState == FidState)
			return BaseCharacterState;
	}

	return nullptr;
}
