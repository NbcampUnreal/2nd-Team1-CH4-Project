// 


#include "Character/StateSystem.h"

#include "Character/AbilityType/AbilityTypeManager.h"
#include "Character/CharacterState/CharacterStateManager.h"


// Sets default values for this component's properties
UStateSystem::UStateSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
	StateManager = CreateDefaultSubobject<UCharacterStateManager>("StateSystem");
	AbilityManager = CreateDefaultSubobject<UAbilityTypeManager>("AbilityManager");
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
	if (StateManager)
	{
		StateManager->ChangeCharacterState(NewState);
	}
}

EPlayerStates UStateSystem::GetCurrentState() const
{
	if (StateManager)
	{
		return StateManager->GetCurrentState();
	}
	return EPlayerStates::Idle;
}

