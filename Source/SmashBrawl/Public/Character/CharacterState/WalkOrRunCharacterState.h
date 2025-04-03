// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "WalkOrRunCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UWalkOrRunCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	UWalkOrRunCharacterState();
	virtual bool CanState() override;
};
