// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/HitBox/SmashMonsterDamager.h"
#include "GameFramework/Actor.h"
#include "BaseBossPatternActor.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseBossPatternActor : public ASmashMonsterDamager
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseBossPatternActor();

	void DestroyMe();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle PatterActorDestroyTimerHandle;
};