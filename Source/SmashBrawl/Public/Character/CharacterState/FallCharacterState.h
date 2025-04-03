// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "FallCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UFallCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UFallCharacterState();
};
