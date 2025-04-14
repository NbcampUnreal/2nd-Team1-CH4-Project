// 


#include "AbilitySystem/HitBox/BaseSmashDamagerManager.h"

#include "AbilitySystem/HitBox/SmashBaseDamager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBaseSmashDamagerManager::UBaseSmashDamagerManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBaseSmashDamagerManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

AActor* UBaseSmashDamagerManager::CreateDamager(FTransform SpawnTransform, const FName& DamagerId)
{
	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		FDamagePlayRow DamagePlayRow;
		if (FDamagePlayRow* TableDamagePlayRow = DamagePlayTable->FindRow<FDamagePlayRow>(DamagerId, TEXT("DamagePlayRow")))
		{

			DamagePlayRow = *TableDamagePlayRow;
			if (ASmashBaseDamager* SmashBaseDamager =
	GetWorld()->SpawnActorDeferred<ASmashBaseDamager>(TableDamagePlayRow->DamagerVisualActor,
		SpawnTransform,
		GetOwner(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				SmashBaseDamager->Init(GetOwner(), AttackAbleClasses, DamagePlayRow);
				if (AActor* SpawnActor =
			UGameplayStatics::FinishSpawningActor(SmashBaseDamager, SpawnTransform))
				{
					return SpawnActor;
				}
			}
		}
	}
	return nullptr;
}
