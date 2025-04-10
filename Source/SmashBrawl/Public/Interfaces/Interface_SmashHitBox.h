// 

#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "UObject/Interface.h"
#include "Interface_SmashHitBox.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_TakeDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_TakeDamage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual bool bHitConditions() = 0;
	virtual void TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection = true) = 0;

};

UINTERFACE()
class UInterface_SmashHitBoxPriority : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_SmashHitBoxPriority
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void ActionPriority(int32 OtherPriority) = 0;
	
	virtual const int32 GetPriority() = 0;
	virtual const int32 ComparatorPriority(int32 OtherPriority) = 0;
};
