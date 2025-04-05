// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BaseBossPatternActor.h"
#include "Lv1BossMonsterLavaBurst.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonsterLavaBurst : public ABaseBossPatternActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1BossMonsterLavaBurst();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* NiagaraTrailEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffectTemplate;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraSideEffectTemplate;
};
