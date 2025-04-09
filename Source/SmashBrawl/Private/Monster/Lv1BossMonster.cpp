// 


#include "Monster/Lv1BossMonster.h"

#include "NiagaraComponent.h"
#include "Charactor/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Monster/BaseBossPatternActor.h"


// Sets default values
ALv1BossMonster::ALv1BossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LeftEyeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LeftEye"));
	LeftEyeNiagaraComponent->SetupAttachment(LeftEyeComp);
	LeftEyeNiagaraComponent->bAutoActivate = false;

	RightEyeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RightEye"));
	RightEyeNiagaraComponent->SetupAttachment(RightEyeComp);
	RightEyeNiagaraComponent->bAutoActivate = false;

	// LeftEye->bAutoActivate = false;
	// RightEye->bAutoActivate = false;
}

void ALv1BossMonster::Server_FireEye_Implementation()
{
	LeftEyeNiagaraComponent->Activate();
	RightEyeNiagaraComponent->Activate();
	// LeftEye->Activate();
	// RightEye->Activate();
}


void ALv1BossMonster::Server_VacuumAttack_Implementation()
{
	FVector HeadLocation = HeadCollision->GetComponentLocation();
	FVector SpawnLocation = FVector(HeadLocation.X, HeadLocation.Y, HeadLocation.Z);
	FRotator SpawnRotation = FRotator(0, 90, 0);
	GetWorld()->SpawnActor<ABaseBossPatternActor>(Vacuum, SpawnLocation, SpawnRotation);
}

void ALv1BossMonster::Server_SideLavaLAttack_Implementation()
{
	FVector LeftArmLocation = LeftArmCollision->GetComponentLocation();
	FVector LeftSpawnLocation = FVector(0, LeftArmLocation.Y, LeftArmLocation.Z);
	FRotator LeftSpawnRotation = FRotator(0, 0, 0);
	GetWorld()->SpawnActor<ABaseBossPatternActor>(SideLavaBurstL, LeftSpawnLocation, LeftSpawnRotation);
}

void ALv1BossMonster::Server_SideLavaRAttack_Implementation()
{
	FVector RightArmLocation = RightArmCollision->GetComponentLocation();
	FVector RightSpawnLocation = FVector(0, RightArmLocation.Y, RightArmLocation.Z);
	FRotator RightSpawnRotation = FRotator(0, 0, 0);
	GetWorld()->SpawnActor<ABaseBossPatternActor>(SideLavaBurstR, RightSpawnLocation, RightSpawnRotation);
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


