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
	virtual void TickState() override;
	virtual FCharacterStateInfo GetStateInfo() override;
	virtual bool CanState() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	EPlayerStates PlayerState;
	FCharacterStateInfo PlayerStateInfo;
};
