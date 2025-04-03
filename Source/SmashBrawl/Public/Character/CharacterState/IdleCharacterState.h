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
	
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual bool CanState() override;


	bool DoExecute =false;
	
};
