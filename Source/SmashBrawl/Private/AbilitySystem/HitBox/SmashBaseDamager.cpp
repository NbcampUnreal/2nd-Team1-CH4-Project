// 


#include "AbilitySystem/HitBox/SmashBaseDamager.h"

#include "Character/SmashCharacter.h"
#include "Interfaces/Interface_SmashHitBox.h"


// Sets default values
ASmashBaseDamager::ASmashBaseDamager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SmashDamageBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Smash Damage Box"));
	SetRootComponent(SmashDamageBox);
	SmashDamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap만
	SmashDamageBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	SmashDamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SmashDamageBox->SetVisibility(true);
}

// Called when the game starts or when spawned
void ASmashBaseDamager::BeginPlay()
{
	Super::BeginPlay();
}


void ASmashBaseDamager::Init(const TObjectPtr<AActor> InParent,
	const TArray<TSubclassOf<AActor>>& InAttackAbleClasses, const FDamagePlayRow& InDamagePlayRow,
	const FDamageVisualRow& InDamageVisualRow)
{
	Parent = InParent;
	AttackAbleClasses = InAttackAbleClasses;
	DamagePlayRow = InDamagePlayRow;
	DamageVisualRow = InDamageVisualRow;

	IgnoreActors.Add(Parent);
	
	if (UStaticMesh* Mesh = Cast<UStaticMesh>(DamageVisualRow.Mesh.LoadSynchronous()))
	{
		SmashDamageBox->SetStaticMesh(Mesh);
		SmashDamageBox->SetWorldScale3D(DamageVisualRow.Scale);
	}
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &ASmashBaseDamager::LifeTimeOut, DamagePlayRow.LifeTime, false);

	SmashDamageBox->OnComponentBeginOverlap.AddDynamic(this, &ASmashBaseDamager::OnMeshBeginOverlap);
	
	TArray<AActor*> OverlappingActors;
	SmashDamageBox->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		OnMeshBeginOverlap(SmashDamageBox, OverlappingActor, nullptr, 0, true, FHitResult());
	}
}

void ASmashBaseDamager::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsAttackAble(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult))
	{
		IgnoreActors.Add(OtherActor);
		AttackActor(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

bool ASmashBaseDamager::bIsAttackAble(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IgnoreActors.Contains(OtherActor))
	{
		if (OtherActor->Tags.Contains("AttackAble"))
		{
			if (IInterface_TakeDamage* TakeDamageOtherActor = Cast<IInterface_TakeDamage>(OtherActor))
			{
				for (TSubclassOf SoftClass : AttackAbleClasses)
				{
					if (SoftClass.Get() && OtherActor->IsA(SoftClass.Get()))
					{
						if (TakeDamageOtherActor->bHitConditions())
						{
							return true;
						}
					}
				}
			}
		}	
	}
	
	return false;
}

void ASmashBaseDamager::AttackActor_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IInterface_TakeDamage* TakeDamageOtherActor = Cast<IInterface_TakeDamage>(OtherActor))
	{
		if (DamagePlayRow.HitDirection == EHitDirection::Left ||
	(DamagePlayRow.HitDirection == EHitDirection::Auto && Owner->GetActorLocation().X - OtherActor->GetActorLocation().X > 0))
		{
			TakeDamageOtherActor->TakeDamage(DamagePlayRow.DamageAmount, DamagePlayRow.AttackType, false);
		}
		else if (DamagePlayRow.HitDirection == EHitDirection::Right ||
			(DamagePlayRow.HitDirection == EHitDirection::Auto && Owner->GetActorLocation().X - OtherActor->GetActorLocation().X < 0))
		{
			TakeDamageOtherActor->TakeDamage(DamagePlayRow.DamageAmount, DamagePlayRow.AttackType, true);
		}
	}
}


void ASmashBaseDamager::LifeTimeOut()
{
	Destroy();
}

