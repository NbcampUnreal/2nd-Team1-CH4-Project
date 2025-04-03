// 

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterState/CharacterStateInterface.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseCharacterState.generated.h"

class ABaseSSTCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SMASHBRAWL_API UBaseCharacterState : public UObject, public IInterface_CharacterState
{
	GENERATED_BODY()

public:
	UBaseCharacterState();

public:
	virtual void EnterState() override;
	virtual void TickState() override;
	virtual void ExitState() override;

public:
	virtual bool CanState() { return true; }

	EPlayerStates GetPlayerState() const;

public:
	EPlayerStates PlayerState;

	UPROPERTY()
	ABaseSSTCharacter* OwnerCharacter;
};
