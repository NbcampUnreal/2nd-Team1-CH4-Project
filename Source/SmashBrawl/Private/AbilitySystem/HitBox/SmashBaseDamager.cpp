// 


#include "AbilitySystem/HitBox/SmashBaseDamager.h"


// Sets default values
ASmashBaseDamager::ASmashBaseDamager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ASmashBaseDamager::Init(ACharacter* Parent, const FHitProperty HitProperty)
{
	
	SpawningParent = Parent;
	SpawningLifeTime = 3.0f;
	SpawningHitProperty = HitProperty;
	SpawningPriority = 0;
	// 캐릭터의 Faceing 추가할것 지금은 임시로 Left
	SpawningDirection = EFaceing::Left;
}


// Called when the game starts or when spawned
void ASmashBaseDamager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmashBaseDamager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

