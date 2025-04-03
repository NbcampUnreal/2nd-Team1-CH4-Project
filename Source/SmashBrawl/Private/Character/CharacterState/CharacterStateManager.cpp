// 


#include "Character/CharacterState/CharacterStateManager.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "Net/UnrealNetwork.h"

UCharacterStateManager::UCharacterStateManager()
{

}

void UCharacterStateManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UBaseCharacterState> Class : CharacterStates)
	{
		if (TObjectPtr<UBaseCharacterState> CharacterState = NewObject<UBaseCharacterState>(this, Class))
		{
			CharacterStatePtrs.Add(CharacterState->GetPlayerState(), CharacterState);
		}
	}
	
	if (const TObjectPtr<UBaseCharacterState>* State = CharacterStatePtrs.Find(EPlayerStates::Idle))
	{
		CurrentState = State->Get();
	}
}

void UCharacterStateManager::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterStateManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UCharacterStateManager, CurrentState);
}

void UCharacterStateManager::ChangeCharacterState(EPlayerStates NewState)
{
	if (CurrentState)
	{
		CurrentState->ExitState();
	}
	if (TObjectPtr<UBaseCharacterState>* CharacterState = CharacterStatePtrs.Find(NewState))
	{
		CurrentState = CharacterState->Get();
		CurrentState->EnterState();
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

void UCharacterStateManager::InitializeComponent()
{
	Super::InitializeComponent();
}
