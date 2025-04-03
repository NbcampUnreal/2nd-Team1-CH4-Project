// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "HeldCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UHeldCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UHeldCharacterState();
};
