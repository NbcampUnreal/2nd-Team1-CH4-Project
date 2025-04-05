// 


#include "Monster/BaseBossPatternActor.h"


// Sets default values
ABaseBossPatternActor::ABaseBossPatternActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseBossPatternActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBossPatternActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

