// 


#include "Monster/Lv1BossMonsterSideLavaL.h"

ALv1BossMonsterSideLavaL::ALv1BossMonsterSideLavaL()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;

	bReplicates = true;
}

void ALv1BossMonsterSideLavaL::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffectTemplate,
			GetActorLocation() + FVector(250.0f, 0, -150.0f),
			FRotator(0, -90, 0),
			FVector(5.0f, 10.0f, 5.0f) 
		);
	}
}
