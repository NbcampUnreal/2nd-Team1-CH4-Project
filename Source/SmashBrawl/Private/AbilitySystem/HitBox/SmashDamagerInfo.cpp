// 


#include "AbilitySystem/HitBox/SmashDamagerInfo.h"


// Sets default values
ASmashDamagerInfo::ASmashDamagerInfo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmashDamagerInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmashDamagerInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

