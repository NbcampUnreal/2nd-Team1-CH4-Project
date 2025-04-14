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
			GetActorLocation() + FVector(0, 0, -150.0f),
			FRotator(0, 180, 0),
			FVector(5.0f, 10.0f, 5.0f) 
		);
	}
	
	SmashDamageBox->SetWorldScale3D(FVector(7.0f, 30.0f, 100.0f));

	DamagerLocationChange();
	
	GetWorldTimerManager().SetTimer(
		DamagerRepeatTimerHandle,
		this,
		&ALv1BossMonsterSideLavaR::DamagerLocationChange,
		0.3f,
		true
	);
}

void ALv1BossMonsterSideLavaR::DamagerDeActive()
{
	SmashDamageBox->SetActive(false);
}

void ALv1BossMonsterSideLavaR::DamagerLocationChange()
{
	if (SmashDamageBox)
	{
		if (DamagerCallCount >= 5)
		{
			GetWorldTimerManager().ClearTimer(DamagerRepeatTimerHandle); // 타이머 중지
			Destroy();
		}
		
		FVector Offset = FVector(-600.0f * DamagerCallCount, 0.0f, 0.0f);
		SmashDamageBox->SetWorldLocation(GetActorLocation() + Offset);
		SmashDamageBox->SetActive(true);

		GetWorldTimerManager().SetTimer(DamagerActiverTimerHandle, this, &ALv1BossMonsterSideLavaR::DamagerDeActive, 0.1f, false);
		
		DamagerCallCount++;

	}
}
