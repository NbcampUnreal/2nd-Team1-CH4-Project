﻿// 


#include "Monster/MobSpawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BaseBossMonster.h"
#include "Monster/MeteorSpawner.h"
#include "Monster/Character/BaseAIFighter.h"


// Sets default values
AMobSpawner::AMobSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	
	SpawnVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolumeComp->SetupAttachment(MeshComp);

	bReplicates = true;
}

FVector AMobSpawner::GetSpanwerVolume() const
{
	FVector BoxExtent = SpawnVolumeComp->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnVolumeComp->GetComponentLocation();
	FVector BoxOriginRemoveX = FVector( BoxOrigin.X, BoxOrigin.Y, BoxOrigin.Z);

	return BoxOriginRemoveX + FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
		);
}

void AMobSpawner::StartActive()
{
	GetWorldTimerManager().SetTimer(MeteorSpawnTimer, this, &AMobSpawner::SpawnMeteor, SpawnTime, true);
}

void AMobSpawner::StopActive()
{
	GetWorldTimerManager().ClearTimer(MeteorSpawnTimer);
}

// Called when the game starts or when spawned
void AMobSpawner::BeginPlay()
{
	Super::BeginPlay();

	BossMonster = Cast<ABaseBossMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseBossMonster::StaticClass()));
	
	if (BossMonster)
	{
		BossMonster->OnTriggerDestructionInit.AddDynamic(this, &AMobSpawner::StartActive);
		BossMonster->OnTriggerDestructionPhase2.AddDynamic(this, &AMobSpawner::StopActive);
	}
}

void AMobSpawner::SpawnMeteor() const
{
	
	FVector SpawnLocation = GetSpanwerVolume();
	FRotator SpawnRotation = FRotator(0, 0, 0);

	GetWorld()->SpawnActor<ABaseAIFighter>(AIMonster, SpawnLocation, SpawnRotation);
			//GetWorld()->SpawnActor<ALv1BossMonsterMeteor>(Projectile, SpawnLocation, SpawnRotation);
}

