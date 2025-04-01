// 

#pragma once

#include "CoreMinimal.h"

#include "Interface_BossMonsterCombat.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_BossMonsterCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_BossMonsterCombat
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PerformAttack() = 0;

	// 피격 반응 (예: 피격 애니메이션)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void ReactToHit() = 0;

	// 죽음 시 호출
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnDeath() = 0;
};
