// 

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterState/CharacterStateInterface.h"
#include "Core/SmashTypes.h"
#include "Components/ActorComponent.h"
#include "BaseCharacterState.generated.h"

class ABaseSSTCharacter;
/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBaseCharacterState : public UActorComponent, public IInterface_CharacterState
{
	GENERATED_BODY()
public:
	UBaseCharacterState();
	EPlayerStates GetPlayerState() const;
	virtual void EnterState(IInterface_CharacterState* BeforeCharacterState) override;
	virtual void ExitState() override;
	virtual FCharacterStateInfo GetStateInfo() override;

protected:
	EPlayerStates PlayerState;

	FCharacterStateInfo PlayerStateInfo;
};
