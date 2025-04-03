// 

#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UStateSystem;

UCLASS()
class SMASHBRAWL_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void BasicAttack();
	void BasicAttackEnd();
	void SpecialAttack();

	UFUNCTION(Server, Reliable)
	void Server_BasicAttack();

	UFUNCTION(BlueprintCallable)
	EPlayerStates GetCurrentState() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStateSystem* StateSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Replicated)
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Replicated)
	bool bCanSmash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	EAbilityTypes AbilityTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bAttackButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bAttackButtonReleased;
};
