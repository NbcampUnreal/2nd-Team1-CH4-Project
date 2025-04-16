// 


#include "Monster/Lv1BossMonsterVacuum.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/PawnIterator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BaseBossMonster.h"


// Sets default values
ALv1BossMonsterVacuum::ALv1BossMonsterVacuum()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);
	
	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;

	bReplicates = true;
	
	SmashDamageBox->SetupAttachment(RootComp);
}

void ALv1BossMonsterVacuum::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraEffectTemplate)
	{
		NiagaraTrailEffect->Activate();
		NiagaraTrailEffect->SetAsset(NiagaraEffectTemplate);
	}

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ALv1BossMonsterVacuum::DestroyVacuum, 2.5f, false);
}

void ALv1BossMonsterVacuum::VacuumAction() const
{
	FVector const SuctionCenter = GetActorLocation();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmashCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
		if (ASmashCharacter* PlayerCharacter = Cast<ASmashCharacter>(Actor))
		{
			float Distance = FVector::Dist(SuctionCenter, PlayerCharacter->GetActorLocation());
			FVector Direction = (SuctionCenter - PlayerCharacter->GetActorLocation()).GetSafeNormal();
			FVector LaunchVelocity = Direction * VacuumForce;
			LaunchVelocity.Y = 0;
			PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
		}
	}
}

void ALv1BossMonsterVacuum::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	VacuumAction();
}

void ALv1BossMonsterVacuum::DestroyVacuum()
{
	Destroy();
}
