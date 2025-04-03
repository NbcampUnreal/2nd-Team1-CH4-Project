// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "AbilityCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UAbilityCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
	public:
	UAbilityCharacterState();
	virtual void EnterState() override;

};
