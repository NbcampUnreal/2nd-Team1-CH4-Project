// 

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterState.h"
#include "Components/ActorComponent.h"
#include "CharacterStateManager.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UCharacterStateManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStateManager();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	void ChangeCharacterState(EPlayerStates NewState);
	UFUNCTION(BlueprintCallable)
	EPlayerStates GetCurrentState() const;
	UFUNCTION(BlueprintCallable)
	bool bCanAttack() const;

	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitializeComponent() override;


	UBaseCharacterState* FindState(EPlayerStates FidState) const;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates", Replicated)
	TObjectPtr<UBaseCharacterState> CurrentState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates")
	TArray<TSubclassOf<UBaseCharacterState>> CharacterStates;
	
	UPROPERTY( BlueprintReadWrite, Category = "PlayerStates")
	TArray<TObjectPtr<UBaseCharacterState>> CharacterStatePtrs;
};
