// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "TumbleCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UTumbleCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState() override;
	UTumbleCharacterState();
};
