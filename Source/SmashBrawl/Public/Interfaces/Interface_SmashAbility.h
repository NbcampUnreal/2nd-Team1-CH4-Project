

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/SmashTypes.h"
#include "Interface_SmashAbility.generated.h"

class ASmashCharacter;
// This class does not need to be modified.
UINTERFACE()
class UInterface_SmashAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_SmashAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashAbility")
	void ISetMovement(bool bCanMove, ASmashCharacter* SmashCharacter);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashAbility")
	void ISetCanAttack(bool bCanAttack, ASmashCharacter* SmashCharacter);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashAbility")
	void ISetJump(bool bCanJump, ASmashCharacter* SmashCharacter);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashAbility")
	void ISetFlip(bool bCanFlip, ASmashCharacter* SmashCharacter);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interface SmashAbility")
	void ISetHitStates(ESmashHitStates State, ASmashCharacter* SmashCharacter);
};