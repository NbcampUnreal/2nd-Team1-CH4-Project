// 


#include "Monster/Lv1BossMonster.h"

#include "Charactor/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Monster/BaseBossPatternActor.h"


// Sets default values
ALv1BossMonster::ALv1BossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ALv1BossMonster::Server_MagicAttack_Implementation()
{

	ABaseCharacter* TargetPlayer = GetRandomPlayer();	
	
	FVector SpawnLocation = TargetPlayer->GetActorLocation();
	FRotator SpawnRotation = FRotator(0, 90, 0);
	GetWorld()->SpawnActor<ABaseBossPatternActor>(Magic, SpawnLocation, SpawnRotation);
}

void ALv1BossMonster::Server_LavaBurstAttack_Implementation() const
{
		FVector SpawnLocation = FVector(0, GetActorLocation().Y, GetActorLocation().Z - 900.0f);
		FRotator SpawnRotation = FRotator(0, 90, 0);
		GetWorld()->SpawnActor<ABaseBossPatternActor>(LavaBurst, SpawnLocation, SpawnRotation);
}

void ALv1BossMonster::Server_FlyBreathAttack_Implementation() const
{
	// FVector SpawnLocation = HeadCollision->GetComponentLocation() + FVector(50, 0, 0);
	// FRotator SpawnRotation = FRotator(FMath::RandRange(-90, -90), 90, 0);
	// GetWorld()->SpawnActor<ALv1BossMonsterMeteor>(FlyBreathProjectile, SpawnLocation, SpawnRotation);
}

void ALv1BossMonster::Server_BiteAttack_Implementation() const
{
	
}

//이 아래는 각 공격 구현 목록
void ALv1BossMonster::Server_BreathAttack_Implementation() const
{
	for (int i = 0; i < 5; ++i)
	{
		FVector HeadLocation = HeadCollision->GetComponentLocation();
		FVector SpawnLocation = FVector(0, HeadLocation.Y, HeadLocation.Z);
		FRotator SpawnRotation = FRotator(FMath::RandRange(0, 360), 90, 0);
		GetWorld()->SpawnActor<ABaseBossPatternActor>(BreathProjectile, SpawnLocation, SpawnRotation);
	}
}


