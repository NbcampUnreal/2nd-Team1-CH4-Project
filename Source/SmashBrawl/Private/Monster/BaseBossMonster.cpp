


#include "../../Public/Monster/BaseBossMonster.h"

#include "Components/SphereComponent.h"
#include "Layers/LayersSubsystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseBossMonster::ABaseBossMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadComp"));
	HeadCollision->SetupAttachment(GetMesh(), FName("Head"));
	HeadCollision->SetSphereRadius(20.f);
	HeadCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	bReplicates = true;

	Tags.Add("Boss");
}

void ABaseBossMonster::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseBossMonster, BossState);
	DOREPLIFETIME(ABaseBossMonster, bIsAttacking);
}


void ABaseBossMonster::PerformAttack_Implementation(int32 MontageIndex)
{
	UAnimMontage* Montage = GetMontage(MontageIndex);
	
	if (HasAuthority())
	{
		Multicast_PerformAnimation(Montage);
	}
}

void ABaseBossMonster::Multicast_PerformAnimation_Implementation(UAnimMontage* Montage)
{
	//이 아래에서 공격 정의 필요

	if (Montage && GetMesh())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	}
}

int32 ABaseBossMonster::GetRandomValueInMontageIndex() const
{

	if (AnimMontages.Num() > 0)
	{
		int32 MontageSize = AnimMontages.Num();
		int32 RandomValue = FMath::RandRange(0, MontageSize - 1);

		return RandomValue;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Montage가 없삼"));
		return 0;
	}
}

void ABaseBossMonster::ReactToHit_Implementation()
{
	IInterface_BossMonsterCombat::ReactToHit_Implementation();
}

void ABaseBossMonster::OnDeath_Implementation()
{
	IInterface_BossMonsterCombat::OnDeath_Implementation();
}

