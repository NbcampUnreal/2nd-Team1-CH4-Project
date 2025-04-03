// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "FreeFallCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UFreeFallCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UFreeFallCharacterState();
};
