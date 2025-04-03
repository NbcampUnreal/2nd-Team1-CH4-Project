// 

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterState/CharacterStateInterface.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBaseCharacterState : public UObject, public IInterface_CharacterState
{
	GENERATED_BODY()

protected:
	EPlayerStates PlayerState;	
public:
	UBaseCharacterState();
	EPlayerStates GetPlayerState() const;
	virtual void EnterState() override;
	virtual void ExitState() override;
};