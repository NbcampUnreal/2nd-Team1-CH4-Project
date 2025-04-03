// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "HoldCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UHoldCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	UHoldCharacterState();
};
