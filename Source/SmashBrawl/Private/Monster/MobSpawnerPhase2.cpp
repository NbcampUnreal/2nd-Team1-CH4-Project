// 


#include "Monster/MobSpawnerPhase2.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BaseBossMonster.h"
#include "Monster/Character/BaseAIFighter.h"


// Sets default values
AMobSpawnerPhase2::AMobSpawnerPhase2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	
	SpawnVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolumeComp->SetupAttachment(MeshComp);

	bReplicates = true;
}

FVector AMobSpawnerPhase2::GetSpanwerVolume() const
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

void AMobSpawnerPhase2::StartActive()
{
	GetWorldTimerManager().SetTimer(MeteorSpawnTimer, this, &AMobSpawnerPhase2::SpawnMeteor, SpawnTime, true);
}

void AMobSpawnerPhase2::StopActive()
{
	GetWorldTimerManager().ClearTimer(MeteorSpawnTimer);
}

// Called when the game starts or when spawned
void AMobSpawnerPhase2::BeginPlay()
{
	Super::BeginPlay();

	BossMonster = Cast<ABaseBossMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseBossMonster::StaticClass()));
	
	if (BossMonster)
	{
		BossMonster->OnTriggerDestructionPhase2.AddDynamic(this, &AMobSpawnerPhase2::StartActive);
	}
}

void AMobSpawnerPhase2::SpawnMeteor() const
{
	
	FVector SpawnLocation = GetSpanwerVolume();
	FRotator SpawnRotation = FRotator(0, 0, 0);

	GetWorld()->SpawnActor<ABaseAIFighter>(AIMonster, SpawnLocation, SpawnRotation);
	//GetWorld()->SpawnActor<ALv1BossMonsterMeteor>(Projectile, SpawnLocation, SpawnRotation);
}


