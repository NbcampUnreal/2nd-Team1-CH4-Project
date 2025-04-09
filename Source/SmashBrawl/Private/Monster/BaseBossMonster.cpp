


#include "../../Public/Monster/BaseBossMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	
	LeftEyeComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftEyeComponent"));
	LeftEyeComp->SetupAttachment(GetMesh(), FName("head"));
	LeftEyeComp->SetCollisionProfileName("OverlapAllDynamic");

	RightEyeComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightEyeComponent"));
	RightEyeComp->SetupAttachment(GetMesh(), FName("head"));
	RightEyeComp->SetCollisionProfileName("OverlapAllDynamic");
	
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

void ABaseBossMonster::StartAttack_Implementation()
{
	HeadCollision->SetCollisionProfileName("NoCollision");
	LeftArmCollision->SetCollisionProfileName("NoCollision");
	RightArmCollision->SetCollisionProfileName("NoCollision");
}

void ABaseBossMonster::EndAttack_Implementation()
{
	HeadCollision->SetCollisionProfileName("BlockAllDynamic");
	LeftArmCollision->SetCollisionProfileName("BlockAllDynamic");
	RightArmCollision->SetCollisionProfileName("BlockAllDynamic");
}

void ABaseBossMonster::Server_DoPhase2_Implementation()
{
	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsBool(FName("bPhase2"), true);
		}
	}
}

void ABaseBossMonster::Server_PlatformDestroy_Implementation()
{
	OnTriggerDestruction.Broadcast();
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

	//만약 체력이 적으면
	Server_DoPhase2();
}

void ABaseBossMonster::OnDeath_Implementation()
{
	IInterface_BossMonsterCombat::OnDeath_Implementation();
}

