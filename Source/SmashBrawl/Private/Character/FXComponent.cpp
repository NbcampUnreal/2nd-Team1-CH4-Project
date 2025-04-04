// 


#include "Character/FXComponent.h"

#include "SmashBrawl/SmashBrawl.h"


// Sets default values for this component's properties
UFXComponent::UFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFXComponent::FXMainLoop()
{
	UE_LOG(LogSmash, Warning, TEXT("구현해야합니다"));
}
