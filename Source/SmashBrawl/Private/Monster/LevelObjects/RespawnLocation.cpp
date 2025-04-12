#include "Monster/LevelObjects/RespawnLocation.h"

// Sets default values
ARespawnLocation::ARespawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARespawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}


