// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "SprintCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API USprintCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	virtual void EnterState() override;
	virtual void TickState() override;
	USprintCharacterState();
};
