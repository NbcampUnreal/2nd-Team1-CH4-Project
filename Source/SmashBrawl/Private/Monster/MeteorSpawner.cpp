// 


#include "Monster/MeteorSpawner.h"

#include "AbilitySystem/HitBox/SmashMonsterDamagerManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BaseBossMonster.h"


// Sets default values
AMeteorSpawner::AMeteorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	
	SpawnVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolumeComp->SetupAttachment(MeshComp);
	
	DamagerManager = CreateDefaultSubobject<USmashMonsterDamagerManager>(TEXT("Damager"));

	bReplicates = true;
}

FVector AMeteorSpawner::GetSpanwerVolume() const
{
	FVector BoxExtent = SpawnVolumeComp->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnVolumeComp->GetComponentLocation();
	FVector BoxOriginRemoveX = FVector( BoxOrigin.X, 0, BoxOrigin.Z);

	return BoxOriginRemoveX + FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		0,
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
		);
}

void AMeteorSpawner::StartActive()
{
	GetWorldTimerManager().SetTimer(MeteorSpawnTimer, this, &AMeteorSpawner::SpawnMeteor, SpawnTime, true);
}

void AMeteorSpawner::StopActive()
{
	GetWorldTimerManager().ClearTimer(MeteorSpawnTimer);
}

// Called when the game starts or when spawned
void AMeteorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	BossMonster = Cast<ABaseBossMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseBossMonster::StaticClass()));
	
	if (BossMonster)
	{
		BossMonster->OnTriggerDestructionPhase2.AddDynamic(this, &AMeteorSpawner::StartActive);
	}
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
		DirectionConversionX = -180;
		break;

	case ESmashSpawnerDirection::Right:
		DirectionConversionX = 0;
		break;

	default:
		DirectionConversionX = -90;
		UE_LOG(LogTemp, Error, TEXT("Invalid SpawnDirection"));
		break;
	}
	
	FVector SpawnLocation = GetSpanwerVolume();
	FRotator SpawnRotation = FRotator(DirectionConversionX, 0, 0);
	
	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	DamagerManager->CreateDamager(SpawnTransform, FName("Meteor"));
	
			//GetWorld()->SpawnActor<ALv1BossMonsterMeteor>(Projectile, SpawnLocation, SpawnRotation);
}
