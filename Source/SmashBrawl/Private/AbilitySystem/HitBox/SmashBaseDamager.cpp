// 


#include "AbilitySystem/HitBox/SmashBaseDamager.h"

#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Character/SmashCharacter.h"
#include "Character/SmashStateSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASmashBaseDamager::ASmashBaseDamager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SmashDamageBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Smash Damage Box"));
	
	LifeTime = 3.0f;
}

void ASmashBaseDamager::Init(AActor* InParent, const FHitProperty InHitProperty)
{
	Parent = InParent;
	HitProperty = InHitProperty;
}


// Called when the game starts or when spawned
void ASmashBaseDamager::BeginPlay()
{
	Super::BeginPlay();

	SmashDamageBox->SetStaticMesh(HitBoxMesh);
	SmashDamageBox->OnComponentBeginOverlap.AddDynamic(this, &ASmashBaseDamager::OnMeshBeginOverlap);
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &ASmashBaseDamager::LifeTimeOut, LifeTime, false);
}

void ASmashBaseDamager::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (TScriptInterface HitModel : HitModels)
	{
		if (HitModel->bHitConditions(OtherActor, OtherComp))
		{
			HitModel->Server_OverlapMesh(OtherActor, OtherComp, bIsRightDirection);
		}
	}
}

void ASmashBaseDamager::LifeTimeOut()
{
	Destroy();
}
