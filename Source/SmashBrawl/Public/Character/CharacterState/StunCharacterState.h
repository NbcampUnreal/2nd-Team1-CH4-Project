// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "StunCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UStunCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	UStunCharacterState();
};
