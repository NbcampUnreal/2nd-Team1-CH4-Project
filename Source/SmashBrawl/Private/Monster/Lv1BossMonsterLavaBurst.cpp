﻿// 


#include "Monster/Lv1BossMonsterLavaBurst.h"


// Sets default values
ALv1BossMonsterLavaBurst::ALv1BossMonsterLavaBurst()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;

	bReplicates = true;
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
			FVector(7.0f, 5.0f, 5.0f) 
		);
	}
}


