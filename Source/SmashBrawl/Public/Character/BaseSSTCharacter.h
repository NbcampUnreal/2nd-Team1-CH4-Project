

#pragma once

#include "CoreMinimal.h"
#include "SSTCharacter.h"
#include "BaseSSTCharacter.generated.h"

class UStateSystem;
/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ABaseSSTCharacter : public ASSTCharacter
{
	GENERATED_BODY()

public:
	ABaseSSTCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual void Move(const struct FInputActionValue& Value) override;
	
	virtual void CrouchDrop() override;
	
	virtual void StopCrouchDrop() override;
	
	virtual void JumpOrDrop() override;
	
	virtual void Dash() override;

	virtual void ReleaseJump() override;

	virtual bool CanJumpInternal_Implementation() const override;

	UFUNCTION()
	void BaseAttack();
	UFUNCTION()
	void SpecialAttack();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseSSTCharacter")
	TObjectPtr<class UStateSystem> CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseSSTCharacter")
	TObjectPtr<class USmashAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseSSTCharacter", Replicated)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseSSTCharacter", Replicated)
	bool bPushed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseSSTCharacter")
	bool bJumpButton = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseSSTCharacter")
	TObjectPtr<UStateSystem> StateSystem;

	
};
