// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "LaunchCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ULaunchCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	ULaunchCharacterState();
};
