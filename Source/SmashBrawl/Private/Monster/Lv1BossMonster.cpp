// 


#include "Monster/Lv1BossMonster.h"

#include "Components/SphereComponent.h"
#include "Monster/Lv1BossMonsterBreathProjectile.h"


// Sets default values
ALv1BossMonster::ALv1BossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


void ALv1BossMonster::PerformAttack_Implementation(UAnimMontage* Montage)
{	
	if (HasAuthority())
	{
		Multicast_PerformAnimation(Montage);
	}
}

void ALv1BossMonster::Multicast_PerformAnimation_Implementation(UAnimMontage* Montage)
{
	//이 아래에서 공격 정의 필요

	if (Montage && GetMesh())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	}
}

//이 아래는 각 공격 구현 목록
void ALv1BossMonster::Server_BreathAttack_Implementation() const
{
	FVector SpawnLocation = HeadComp->GetComponentLocation();
	FRotator SpawnRotation = FRotator(FMath::RandRange(0, 360), 90, 0);
	GetWorld()->SpawnActor<ALv1BossMonsterBreathProjectile>(BreathProjectile, SpawnLocation, SpawnRotation);
}

void ALv1BossMonster::ReactToHit_Implementation()
{
}

void ALv1BossMonster::OnDeath_Implementation()
{
}

