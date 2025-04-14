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

	SmashDamageBox->SetupAttachment(RootComp);
}

void ALv1BossMonsterSideLavaL::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffectTemplate,
			GetActorLocation() + FVector(0, 0, -150.0f),
			FRotator(0, 0, 0),
			FVector(5.0f, 10.0f, 5.0f) 
		);
	}

	SmashDamageBox->SetWorldScale3D(FVector(5.0f, 30.0f, 100.0f));

	DamagerLocationChange();
	
	GetWorldTimerManager().SetTimer(
		DamagerRepeatTimerHandle,
		this,
		&ALv1BossMonsterSideLavaL::DamagerLocationChange,
		0.3f,
		true
	);

}

void ALv1BossMonsterSideLavaL::DamagerDeActive()
{
	SmashDamageBox->SetActive(false);
}

void ALv1BossMonsterSideLavaL::DamagerLocationChange()
{
	if (SmashDamageBox)
	{
		FVector Offset = FVector(600.0f * DamagerCallCount, 0.0f, 0.0f);
		SmashDamageBox->SetWorldLocation(GetActorLocation() + Offset);
		SmashDamageBox->SetActive(true);

		GetWorldTimerManager().SetTimer(DamagerActiverTimerHandle, this, &ALv1BossMonsterSideLavaL::DamagerDeActive, 0.1f, false);
		
		DamagerCallCount++;
		if (DamagerCallCount >= 5)
		{
			GetWorldTimerManager().ClearTimer(DamagerRepeatTimerHandle); // 타이머 중지
			Destroy();
		}
	}
}
