// 

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BaseBossPatternActor.h"
#include "Lv1BossMonsterSideLavaR.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonsterSideLavaR : public ABaseBossPatternActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1BossMonsterSideLavaR();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* NiagaraTrailEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffectTemplate;
};
