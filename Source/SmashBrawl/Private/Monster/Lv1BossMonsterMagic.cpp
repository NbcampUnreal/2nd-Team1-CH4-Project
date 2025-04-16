// 


#include "Monster/Lv1BossMonsterMagic.h"

#include "NiagaraComponent.h"


// Sets default values
ALv1BossMonsterMagic::ALv1BossMonsterMagic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	TelegraphMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TelegraphBoxComponent"));
	TelegraphMeshComponent->SetupAttachment(RootComp);
	
	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;

	bReplicates = true;
	
	SmashDamageBox->SetupAttachment(RootComp);
	SmashDamageBox->SetActive(false);
	SmashDamageBox->SetWorldLocation(GetActorLocation() + FVector(1, 1000.0f, 1.0f));
}

void ALv1BossMonsterMagic::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(DoAttackTimerHandle, this, &ALv1BossMonsterMagic::DoAttack, 1.5f, false);
}

void ALv1BossMonsterMagic::DoAttack()
{
	if (NiagaraEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffectTemplate,
			GetActorLocation() - FVector(0, 0, 0),
			FRotator(0, FMath::RandRange(0, 360), 0),
			FVector(5.0f, 5.0f, 5.0f) 
		);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	GetWorld(),
	NiagaraEffectTemplate,
	GetActorLocation() - FVector(0, 0, 0),
	FRotator(180, FMath::RandRange(0, 360), 0),
	FVector(2.0f, 2.0f, 2.0f) 
);
	}

	TelegraphMeshComponent->SetHiddenInGame(true);
	
	if (SmashDamageBox)
	{
		SmashDamageBox->SetWorldLocation(GetActorLocation() + FVector(0, 0, 200.0f));
		SmashDamageBox->SetActive(true);
		SmashDamageBox->SetWorldScale3D(FVector(4.0f, 30.0f, 5.0f));
	}
	
	// TArray<AActor*> OverlappingActors;
	// SmashDamageBox->GetOverlappingActors(OverlappingActors);
	//
	// for (AActor* OverlappingActor : OverlappingActors)
	// {
	// 	OnMeshBeginOverlap(SmashDamageBox, OverlappingActor, nullptr, 0, true, FHitResult());
	// }
}
