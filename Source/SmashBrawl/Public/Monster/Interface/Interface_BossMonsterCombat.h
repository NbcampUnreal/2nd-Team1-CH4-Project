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
	void PerformAttack(int32 MontageIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void ReactToHit();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnDeath();
};
