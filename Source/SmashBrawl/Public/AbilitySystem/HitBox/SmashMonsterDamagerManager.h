// 

#pragma once

#include "CoreMinimal.h"
#include "BaseSmashDamagerManager.h"
#include "SmashMonsterDamagerManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashMonsterDamagerManager : public UBaseSmashDamagerManager
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashMonsterDamagerManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
};
