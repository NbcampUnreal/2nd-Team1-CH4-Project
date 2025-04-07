// 

#pragma once

#include "CoreMinimal.h"
#include "BaseBossPatternActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Lv1BossMonsterSideLavaL.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonsterSideLavaL : public ABaseBossPatternActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1BossMonsterSideLavaL();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* RootComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* NiagaraTrailEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffectTemplate;
};
