#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseCharacterState.generated.h"

class UStateSystem;
class ABaseSSTCharacter;
/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBaseCharacterState : public UObject
{
	GENERATED_BODY()

public:
	UBaseCharacterState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void InitState(UStateSystem* InStateSystem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void EnterState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void ExitState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void TickState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	bool CanState();

	UFUNCTION(BlueprintCallable, Category="Character State")
	void SetCapsuleSize(float HalfHeight, bool bUpdateOverlap);


public:
	UFUNCTION(blueprintpure, Category = "Character State")
	EPlayerStates GetPlayerState() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character State", meta=(AllowPrivateAccess=true))
	EPlayerStates PlayerState;

	UPROPERTY(BlueprintReadWrite, Category="Character State", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStateSystem> OwnerStateSystem;

	UPROPERTY(BlueprintReadWrite, Category="Character State", meta=(AllowPrivateAccess=true))
	TObjectPtr<ABaseSSTCharacter> OwnerCharacter;
};
