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

	UFUNCTION(BlueprintCallable)
	ESmashPlayerStates GetCurrentState() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStateSystem* StateSystem;

};
