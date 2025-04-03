// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityTypeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_AbilityTypeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_AbilityTypeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void EnterState() = 0;
	virtual void ExitState() = 0;
};
