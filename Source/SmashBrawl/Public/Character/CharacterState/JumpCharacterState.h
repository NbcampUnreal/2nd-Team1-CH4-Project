// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "JumpCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UJumpCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	UJumpCharacterState();
};
