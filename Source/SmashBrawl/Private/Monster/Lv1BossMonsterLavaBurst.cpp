// 


#include "Monster/Lv1BossMonsterLavaBurst.h"


// Sets default values
ALv1BossMonsterLavaBurst::ALv1BossMonsterLavaBurst()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;

	bReplicates = true;
	
	SmashDamageBox->SetupAttachment(RootComp);
}

void ALv1BossMonsterLavaBurst::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffectTemplate,
			GetActorLocation(),
			GetActorRotation(),
			FVector(10.0f, 5.0f, 5.0f) 
		);
	}

	if (SmashDamageBox)
	{
		SmashDamageBox->SetWorldScale3D(FVector(15.0f, 30.0f, 100.0f));
	}
}


