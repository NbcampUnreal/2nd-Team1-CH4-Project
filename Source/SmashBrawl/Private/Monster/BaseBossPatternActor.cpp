// 


#include "Monster/BaseBossPatternActor.h"


// Sets default values
ABaseBossPatternActor::ABaseBossPatternActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

void ABaseBossPatternActor::DestroyMe()
{
	Destroy();
}

void ABaseBossPatternActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(PatterActorDestroyTimerHandle, this, &ABaseBossPatternActor::DestroyMe, 10.0f, false);
}

void ABaseBossPatternActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(PatterActorDestroyTimerHandle);
}



