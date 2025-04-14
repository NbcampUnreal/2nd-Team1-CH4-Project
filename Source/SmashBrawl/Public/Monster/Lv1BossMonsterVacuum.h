// 

#pragma once

#include "CoreMinimal.h"
#include "BaseBossPatternActor.h"
#include "NiagaraComponent.h"
#include "Lv1BossMonsterVacuum.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonsterVacuum : public ABaseBossPatternActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1BossMonsterVacuum();

	virtual void BeginPlay() override;

	UFUNCTION()
	void VacuumAction() const;

	virtual void Tick(float DeltaSeconds) override;

	void DestroyVacuum();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* NiagaraTrailEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffectTemplate;

	UPROPERTY(EditAnywhere, Category = "Value")
	float VacuumForce = 2000.f;

	FTimerHandle DestroyTimer;

};
