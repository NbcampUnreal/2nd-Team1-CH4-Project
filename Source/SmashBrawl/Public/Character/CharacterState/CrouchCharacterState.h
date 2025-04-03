// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "CrouchCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UCrouchCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	UCrouchCharacterState();
};
