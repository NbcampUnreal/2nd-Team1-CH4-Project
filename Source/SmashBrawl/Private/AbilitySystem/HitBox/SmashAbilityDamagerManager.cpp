// 


#include "AbilitySystem/HitBox/SmashAbilityDamagerManager.h"

#include "AbilitySystem/HitBox/SmashDamagerInfo.h"
#include "AbilitySystem/HitBox/SmashPlayerDamager.h"
#include "Core/SmashDamageBoxType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values for this component's properties
USmashAbilityDamagerManager::USmashAbilityDamagerManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USmashAbilityDamagerManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	TArray<AActor*> ChildActors;
	GetOwner()->GetAllChildActors(ChildActors);
	for (AActor* ChildActor : ChildActors)
	{
		if (ASmashDamagerInfo* DamagerInfo = Cast<ASmashDamagerInfo>(ChildActor))
		{
			FSmashDamagerInfoProperty SmashDamagerInfoProperty;
			if (DamagePlayTable)
			{
				if (FDamagePlayRow* DamagePlayRow = DamagePlayTable->FindRow<FDamagePlayRow>(DamagerInfo->SmashAttackRow, TEXT("SmashAttackRow")))
				{
					SmashDamagerInfoProperty.DamagePlayRow.Attack =	DamagePlayRow->Attack;
					SmashDamagerInfoProperty.DamagePlayRow.DamageAmount = DamagePlayRow->DamageAmount;
					SmashDamagerInfoProperty.DamagePlayRow.KnockbackMultiplier = DamagePlayRow->KnockbackMultiplier;
					SmashDamagerInfoProperty.DamagePlayRow.AttackType = DamagePlayRow->AttackType;
				}
			}
			float TmpLocation = DamagerInfo->RelativeLocation.X;
			DamagerInfo->RelativeLocation.X = DamagerInfo->RelativeLocation.Y;
			DamagerInfo->RelativeLocation.Y = TmpLocation;
			
			SmashDamagerInfoProperty.RelativeSpawnTransform = FTransform(DamagerInfo->SmashRotator, DamagerInfo->RelativeLocation);
			SmashDamagerInfoProperty.RelativeSpawnTransform.SetScale3D(DamagerInfo->GetActorTransform().GetScale3D());

			UE_LOG(LogTemp, Display, TEXT("%s"), *DamagerInfo->GetActorTransform().GetScale3D().ToString());
			SmashDamagerInfoProperty.DamagePlayRow.HitDirection = DamagerInfo->HitDirection;
			SmashDamagerInfoProperty.DamagePlayRow.LifeTime = 3.0f;

			SmashDamagerInfoProperty.DamageVisualRow.Mesh = DamagerInfo->Mesh;
			SmashDamagerInfoProperty.DamageVisualRow.SpawnSound = DamagerInfo->SpawnSound;
			SmashDamagerInfoProperty.DamageVisualRow.Particle = DamagerInfo->Particle;

			DamagerInfos.Add(SmashDamagerInfoProperty);
		}
	}
}

TArray<TObjectPtr<AActor>> USmashAbilityDamagerManager::SpawnDamagerAll()
{
	TArray<TObjectPtr<AActor>> SpawnDamagerAll;
	for (FSmashDamagerInfoProperty DamagerInfoProperty : DamagerInfos)
	{
		if (ASmashBaseDamager* SmashBaseDamager =
			GetWorld()->SpawnActorDeferred<ASmashBaseDamager>(ASmashPlayerDamager::StaticClass(),
				DamagerInfoProperty.RelativeSpawnTransform,
				GetOwner(),
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			SmashBaseDamager->Init(GetOwner(), AttackAbleClasses, DamagerInfoProperty.DamagePlayRow, DamagerInfoProperty.DamageVisualRow);
			if (AActor* SpawnActor =
		UGameplayStatics::FinishSpawningActor(SmashBaseDamager, DamagerInfoProperty.RelativeSpawnTransform))
			{
				UE_LOG(LogTemp, Display, TEXT("%s"), *DamagerInfoProperty.RelativeSpawnTransform.GetScale3D().ToString());
				SpawnActor->SetActorScale3D(DamagerInfoProperty.RelativeSpawnTransform.GetScale3D());
				SpawnActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
				SpawnDamagerAll.Add(SpawnActor);
			}
		}
	}

	return SpawnDamagerAll;
}



// Called every frame
void USmashAbilityDamagerManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

