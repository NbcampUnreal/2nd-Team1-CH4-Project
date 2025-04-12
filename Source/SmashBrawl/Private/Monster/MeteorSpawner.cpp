// 


#include "Monster/MeteorSpawner.h"

#include "Components/BoxComponent.h"


// Sets default values
AMeteorSpawner::AMeteorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	
	SpawnVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolumeComp->SetupAttachment(MeshComp);

	bReplicates = true;
}

FVector AMeteorSpawner::GetSpanwerVolume() const
{
	FVector BoxExtent = SpawnVolumeComp->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnVolumeComp->GetComponentLocation();
	FVector BoxOriginRemoveX = FVector(0, BoxOrigin.Y, BoxOrigin.Z);

	return BoxOriginRemoveX + FVector(
		0,
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
		);
}

// Called when the game starts or when spawned
void AMeteorSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MeteorSpawnTimer, this, &AMeteorSpawner::SpawnMeteor, SpawnTime, true);
}

void AMeteorSpawner::SpawnMeteor() const
{

	int32 DirectionConversionX = -90;
	
	switch (SpawnDirection)
	{
	case ESmashSpawnerDirection::Down:
		DirectionConversionX = -90;
		break;

	case ESmashSpawnerDirection::Up:
		DirectionConversionX = -270;
		break;

	case ESmashSpawnerDirection::Left:
		DirectionConversionX = 0;
		break;

	case ESmashSpawnerDirection::Right:
		DirectionConversionX = -180;
		break;

	default:
		DirectionConversionX = -90;
		UE_LOG(LogTemp, Error, TEXT("Invalid SpawnDirection"));
		break;
	}
	
			FVector SpawnLocation = GetSpanwerVolume();
			FRotator SpawnRotation = FRotator(DirectionConversionX, 90, 0);
	
			GetWorld()->SpawnActor<ALv1BossMonsterMeteor>(Projectile, SpawnLocation, SpawnRotation);
}
