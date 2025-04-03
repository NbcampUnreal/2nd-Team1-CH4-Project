// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStateManager.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UCharacterStateManager : public UActorComponent
{
	GENERATED_BODY()

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
	virtual void InitializeComponent() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates", Replicated)
	TObjectPtr<UBaseCharacterState> CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates")
	TArray<TSubclassOf<UBaseCharacterState>> CharacterStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStates")
	TMap<EPlayerStates, TObjectPtr<UBaseCharacterState>> CharacterStatePtrs;
};
