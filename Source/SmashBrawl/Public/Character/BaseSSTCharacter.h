

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BaseAttack();
	UFUNCTION()
	void SpecialAttack();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateSystem")
	UStateSystem* StateSystem;

	
};
