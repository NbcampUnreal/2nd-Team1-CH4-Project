#include "Monster/BaseFracturePlatform.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BaseBossMonster.h"

ABaseFracturePlatform::ABaseFracturePlatform()
{
	PrimaryActorTick.bCanEverTick = false;
	
	if (UGeometryCollectionComponent* GCComponent = GetGeometryCollectionComponent())
	{
		GCComponent->SetSimulatePhysics(true);
		GCComponent->SetEnableGravity(false);
		GCComponent->SetCollisionProfileName("NoCollision");
		GCComponent->SetCollisionObjectType(ECC_WorldDynamic);
	}

	bReplicates = true;
}

void ABaseFracturePlatform::BeginPlay()
{
	Super::BeginPlay();

	BossMonster = Cast<ABaseBossMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseBossMonster::StaticClass()));
	
	if (BossMonster)
	{
		BossMonster->OnTriggerDestruction.AddDynamic(this, &ABaseFracturePlatform::MultiCast_Destruction);
	}
}

void ABaseFracturePlatform::MultiCast_Destruction_Implementation()
{
	if (UGeometryCollectionComponent* GCComponent = GetGeometryCollectionComponent())
	{
		// 파괴 직전 설정 변경
		GCComponent->SetEnableGravity(true); // 중력 다시 켜줌
		GCComponent->SetSimulatePhysics(true);
		GCComponent->SetNotifyRigidBodyCollision(true);
		GCComponent->SetCollisionProfileName("OverlapOnlyPawn");
		GCComponent->SetCollisionObjectType(ECC_WorldDynamic);
		// 일정 시간 후 제거
		GetWorldTimerManager().SetTimer(
			PlatformDestroyTimerHandle,
			this,
			&ABaseFracturePlatform::DestroyPlatform,
			15.0f,
			false
		);
	}
}

void ABaseFracturePlatform::DestroyPlatform()
{
	Destroy();
}


