// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "ShieldCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UShieldCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UShieldCharacterState();
};
