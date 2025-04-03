// 

#pragma once

#include "CoreMinimal.h"
#include "CharacterState/IdleCharacterState.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "StateSystem.generated.h"


class UCharacterStateManager;
class UBaseCharacterState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UStateSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStateSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void ChangeCharacterState(EPlayerStates NewState);
	UFUNCTION(BlueprintCallable)
	EPlayerStates GetCurrentState() const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates")
	TObjectPtr<UCharacterStateManager> StateManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates")
	TObjectPtr<UCharacterStateManager> AbilityManager;
	
};
