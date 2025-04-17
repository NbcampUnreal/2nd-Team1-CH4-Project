// 


#include "AbilitySystem/HitBox/SmashAIDamagerManager.h"

#include "AbilitySystem/HitBox/SmashDamagerInfo.h"
#include "AbilitySystem/HitBox/SmashPlayerDamager.h"
#include "Core/DamageTable.h"
#include "Core/SmashDamageBoxType.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USmashAIDamagerManager::USmashAIDamagerManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashAIDamagerManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	TArray<AActor*> ChildActors;
	GetOwner()->GetAllChildActors(ChildActors);
	SkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	Parent = GetOwner();
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

			SmashDamagerInfoProperty.DamagePlayRow.HitDirection = DamagerInfo->HitDirection;
			SmashDamagerInfoProperty.DamagePlayRow.LifeTime = 1.0f;

			SmashDamagerInfoProperty.DamageVisualRow.Mesh = DamagerInfo->Mesh;
			SmashDamagerInfoProperty.DamageVisualRow.SpawnSound = DamagerInfo->SpawnSound;
			SmashDamagerInfoProperty.DamageVisualRow.Particle = DamagerInfo->Particle;

			DamagerInfos.Add(SmashDamagerInfoProperty);
		}
	}
}

TArray<TObjectPtr<AActor>> USmashAIDamagerManager::SpawnDamagerAll()
{
	TArray<TObjectPtr<AActor>> SpawnDamagerAll;
	for (FSmashDamagerInfoProperty DamagerInfoProperty : DamagerInfos)
	{
		if (ASmashBaseDamager* SmashBaseDamager =
			GetWorld()->SpawnActorDeferred<ASmashBaseDamager>(ASmashBaseDamager::StaticClass(),
				DamagerInfoProperty.RelativeSpawnTransform,
				Parent,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			SmashBaseDamager->Init(Parent, AttackAbleClasses, DamagerInfoProperty.DamagePlayRow, DamagerInfoProperty.DamageVisualRow);

			if (AActor* SpawnActor =
		UGameplayStatics::FinishSpawningActor(SmashBaseDamager, DamagerInfoProperty.RelativeSpawnTransform))
			{
				SpawnActor->SetActorScale3D(DamagerInfoProperty.RelativeSpawnTransform.GetScale3D());
				SpawnActor->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
				SpawnDamagerAll.Add(SpawnActor);
				if (ASmashBaseDamager* SpawnDamagerActor = Cast<ASmashBaseDamager>(SpawnActor))
				{
					SpawnDamagerActor->DetectOverlapActor();
				}
			}
		}
	}

	return SpawnDamagerAll;
}
