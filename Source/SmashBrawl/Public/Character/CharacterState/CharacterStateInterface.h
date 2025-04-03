// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStateInterface.generated.h"

USTRUCT()
struct FCharacterStateInfo
{
	GENERATED_BODY()

	UPROPERTY()
	bool bCanMove;
	
	UPROPERTY()
	bool bCanSprint;

	UPROPERTY()
	bool bCanFlip;
	
	UPROPERTY()
	bool bCanAttack;
	
	UPROPERTY()
	bool bCanJump;
	
	UPROPERTY()
	int32 JumpNumber;
};

// This class does not need to be modified.
UINTERFACE()
class UInterface_CharacterState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHBRAWL_API IInterface_CharacterState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) = 0;
	virtual void ExitState() = 0;
	virtual FCharacterStateInfo GetStateInfo() = 0;
	virtual bool CanState() = 0;
	virtual void TickState() = 0;

};
