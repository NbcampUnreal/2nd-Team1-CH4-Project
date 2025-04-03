// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "ProneCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UProneCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	UProneCharacterState();
};
