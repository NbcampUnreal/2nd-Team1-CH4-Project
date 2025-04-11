// 


#include "Monster/Lv1BossMonsterSideLavaR.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ALv1BossMonsterSideLavaR::ALv1BossMonsterSideLavaR()
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

void ALv1BossMonsterSideLavaR::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffectTemplate,
			GetActorLocation() + FVector(250.0f, 0, -150.0f),
			FRotator(0, 90, 0),
			FVector(5.0f, 10.0f, 5.0f) 
		);
	}
}
