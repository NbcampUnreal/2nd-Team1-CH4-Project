// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "DizzyCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UDizzyCharacterState : public UBaseCharacterState
{
	GENERATED_BODY()
public:
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	UDizzyCharacterState();
};
