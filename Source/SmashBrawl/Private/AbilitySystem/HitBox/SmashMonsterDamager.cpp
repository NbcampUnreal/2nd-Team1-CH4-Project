// 


#include "AbilitySystem/HitBox/SmashMonsterDamager.h"


// Sets default values
ASmashMonsterDamager::ASmashMonsterDamager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASmashMonsterDamager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmashMonsterDamager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

