


#include "../../Public/Monster/BaseBossMonster.h"

#include "Charactor/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Layers/LayersSubsystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseBossMonster::ABaseBossMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadComp"));
	HeadCollision->SetupAttachment(GetMesh(), FName("head"));
	HeadCollision->SetSphereRadius(20.f);
	HeadCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	LeftArmCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArm"));
	LeftArmCollision->SetupAttachment(GetMesh(), FName("hand_l"));
	LeftArmCollision->SetCapsuleRadius(20.0f);
	LeftArmCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	RightArmCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArm"));
	RightArmCollision->SetupAttachment(GetMesh(), FName("hand_r"));
	RightArmCollision->SetCapsuleRadius(20.0f);
	RightArmCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	

	bReplicates = true;

	Tags.Add("Boss");
}

void ABaseBossMonster::BeginPlay()
{
	Super::BeginPlay();

	CastPlayer();
}

void ABaseBossMonster::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseBossMonster, BossState);
	DOREPLIFETIME(ABaseBossMonster, bIsAttacking);
}

void ABaseBossMonster::CastPlayer()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(Actor))
		{
			PlayerArray.Add(Character);
		}
	}
}

ABaseCharacter* ABaseBossMonster::GetRandomPlayer()
{
	if (PlayerArray.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PlayerArray.Num() - 1);
		ABaseCharacter* RandomCharacter = PlayerArray[RandomIndex];

		return RandomCharacter;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerArray에 Player 없음"));
		return nullptr;
	}
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

