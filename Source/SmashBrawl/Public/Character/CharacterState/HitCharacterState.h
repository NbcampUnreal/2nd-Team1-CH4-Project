// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "HitCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UHitCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UHitCharacterState();
};
