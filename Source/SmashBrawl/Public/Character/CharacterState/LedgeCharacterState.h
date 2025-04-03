// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "LedgeCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ULedgeCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	ULedgeCharacterState();
};
