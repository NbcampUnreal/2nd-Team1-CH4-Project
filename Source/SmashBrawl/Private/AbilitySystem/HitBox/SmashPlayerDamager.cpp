// 


#include "AbilitySystem/HitBox/SmashPlayerDamager.h"

#include "Character/SmashCharacter.h"

// Sets default values
ASmashPlayerDamager::ASmashPlayerDamager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASmashPlayerDamager::BeginPlay()
{
	Super::BeginPlay();
	
}

const int32 ASmashPlayerDamager::GetPriority()
{
	return Priority;
}

const int32 ASmashPlayerDamager::ComparatorPriority(int32 OtherPriority)
{
	return FMath::Clamp(Priority - OtherPriority, -1, 1);
}

void ASmashPlayerDamager::ActionPriority(int32 OtherPriority)
{
	if (ComparatorPriority(OtherPriority) == Priority)
	{
		Flinch();
	}
	else
	{
		Destroy();
	}
}

void ASmashPlayerDamager::Flinch()
{
	Destroy();
}

void ASmashPlayerDamager::AttackActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	int32 TmpDamageAmount = DamagePlayRow.DamageAmount;
	if (ASmashCharacter* OtherSmashActor = Cast<ASmashCharacter>(OtherActor))
	{
		DamagePlayRow.DamageAmount = 0;
	}
	Super::AttackActor(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (ASmashCharacter* OtherSmashActor = Cast<ASmashCharacter>(OtherActor))
	{
		DamagePlayRow.DamageAmount = TmpDamageAmount;
	}

	if (OtherActor->ActorHasTag("Boss"))
	{
		//이펙트 생성
	}
}
