// 

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterState/BaseCharacterState.h"
#include "IdleCharacterState.generated.h"
/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UIdleCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UIdleCharacterState();
};
