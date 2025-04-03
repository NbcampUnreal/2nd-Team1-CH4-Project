// 


#include "Character/StateSystem.h"

#include "Character/AbilityType/AbilityTypeManager.h"


// Sets default values for this component's properties
UStateSystem::UStateSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UStateSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UStateSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateSystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UStateSystem::ChangeCharacterState(EPlayerStates NewState)
{

}

EPlayerStates UStateSystem::GetCurrentState() const
{
	return EPlayerStates::Idle;
}

void UStateSystem::ChangeAbilityType(EAbilityTypes NewState)
{
	if (AbilityManager)
	{
		AbilityManager->ChangeCurrentAbilityType(NewState);
	}
}

EAbilityTypes UStateSystem::GetAbilityType() const
{
	if (AbilityManager)
	{
		return AbilityManager->GetCurrentAbilityType();
	}
	UE_LOG(LogTemp, Error, TEXT("UStateSystem::GetAbilityType Is None"));
	return EAbilityTypes::None;
}

ESmashDirection UStateSystem::GetSmashDirection() const
{
	return CurrentSmashDirection;
}

bool UStateSystem::GetSameState(EPlayerStates ChecktoState)
{
	return GetCurrentState() == ChecktoState;
}

void UStateSystem::ChangeSmashDirection(ESmashDirection SmashDirection)
{
	CurrentSmashDirection = SmashDirection;
}


