// 


#include "Character/AbilityType/AbilityTypeManager.h"

#include "Character/AbilityType/BaseAbilityType.h"
#include "Core/SmashTypes.h"


// Sets default values for this component's properties
UAbilityTypeManager::UAbilityTypeManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityTypeManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for (TSubclassOf<UBaseAbilityType> Class : ValidAbilityTypeClasses)
	{
		if (TObjectPtr<UBaseAbilityType> AbilityType = NewObject<UBaseAbilityType>(this, Class))
		{
			ValidAbilityTypePtrs.Add(AbilityType->GetAbilityType(), AbilityType);
		}
	}
	
	if (const TObjectPtr<UBaseAbilityType>* State = ValidAbilityTypePtrs.Find(EAbilityTypes::None))
	{
		CurrentAbilityType = State->Get();
	}
}


// Called every frame
void UAbilityTypeManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilityTypeManager::ChangeCurrentAbilityType(EAbilityTypes AbilityTypes)
{
	if (CurrentAbilityType)
	{
		CurrentAbilityType->ExitState();
	}
	if (TObjectPtr<UBaseAbilityType>* CharacterState = ValidAbilityTypePtrs.Find(AbilityTypes))
	{
		CurrentAbilityType = CharacterState->Get();
		CurrentAbilityType->EnterState();
	}
}

EAbilityTypes UAbilityTypeManager::GetCurrentAbilityType() const
{
	if (CurrentAbilityType)
	{
		return CurrentAbilityType->GetAbilityType();
	}
	return EAbilityTypes::None;
}

void UAbilityTypeManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

