// 


#include "AbilitySystem/HitBox/SmashAbilityHitboxComponent.h"

#include "AbilitySystem/HitBox/SmashBaseDamager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USmashAbilityHitboxComponent::USmashAbilityHitboxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashAbilityHitboxComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USmashAbilityHitboxComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USmashAbilityHitboxComponent::SpawnDamageBox(int32 CollisionSet)
{
	if (HitProperties.IsValidIndex(CollisionSet))
	{
		SetBoxTransform(CollisionSet);
		FHitProperty HitProperty = HitProperties[CollisionSet];
		if (HitProperty.GrabBox)
		{
			SpawnHitBox(HitProperty);
		}
		else
		{
			SpawnGrabBox(HitProperty);
		}
	}
}

void USmashAbilityHitboxComponent::SpawnHitBox(FHitProperty HitProperty)
{
	if (GetWorld() && SpawnHitClass)
	{
		if (ASmashBaseDamager* SmashBaseDamager =
			GetWorld()->SpawnActorDeferred<ASmashBaseDamager>(SpawnHitClass,
				HitProperty.BoxTransform,
				Parent,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			SmashBaseDamager->Init(Parent, HitProperty);
			SmashBaseDamager->bSpawnByClass = true;
			if (AActor* SpawnActor =
				UGameplayStatics::FinishSpawningActor(SmashBaseDamager, HitProperty.BoxTransform))
			{
				SpawnActor->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	}
}

void USmashAbilityHitboxComponent::SpawnGrabBox(FHitProperty HitProperty)
{
	if (GetWorld() && SpawnHitClass)
	{
		if (ASmashBaseDamager* SmashBaseDamager =
			GetWorld()->SpawnActorDeferred<ASmashBaseDamager>(SpawnGrabClass,
				HitProperty.BoxTransform,
				Parent,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			SmashBaseDamager->Init(Parent, HitProperty);
			SmashBaseDamager->bSpawnByClass = true;

			if (AActor* SpawnActor =
				UGameplayStatics::FinishSpawningActor(SmashBaseDamager, HitProperty.BoxTransform))
			{
				SpawnActor->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	}
}

void USmashAbilityHitboxComponent::SetBoxTransform(int32 CollisionSet)
{
}

void USmashAbilityHitboxComponent::AddHitProperty(FHitProperty HitProperty)
{
}

void USmashAbilityHitboxComponent::FixHitProperty(int32 CollisionSet, FHitProperty HitProperty)
{
}

FHitProperty USmashAbilityHitboxComponent::GetHitProperty(int32 CollisionSet)
{
	if (HitProperties.IsValidIndex(CollisionSet))
	{
		return HitProperties[CollisionSet];
	}
	UE_LOG(LogTemp, Error, TEXT("HitProperty doesn't exist Or ExitRange"));
	return FHitProperty();
}
