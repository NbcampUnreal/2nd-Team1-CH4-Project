// 

#pragma once

#include "CoreMinimal.h"
#include "SmashBrawl/Public/Core/SmashTypes.h"
#include "UObject/Interface.h"
#include "Interface_SmashCombat.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_SmashCombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_SmashCombat
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	ESmashPlayerStates GetPlayerState();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	ESmashAttacks GetAttackTypes();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	ESmashAbilityTypes GetAbilityTypes();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	ESmashDirection GetDirection();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	int32 GetSuper();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	FTransform GetAbilitySpawnTransform();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	void SetSuper(int32 NewSuper);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashCombat")
	void SetAttacks(ESmashAttacks NewAttacks);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashInput")
	void BufferButtons();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashInput")
	void ClearBuffer();
};
