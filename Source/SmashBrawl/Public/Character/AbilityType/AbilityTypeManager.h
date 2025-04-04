// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityTypeManager.generated.h"


enum class ESmashAbilityTypes : uint8;
class UBaseAbilityType;
class IInterface_AbilityTypeInterface;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UAbilityTypeManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityTypeManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeCurrentAbilityType(ESmashAbilityTypes AbilityTypes);
	ESmashAbilityTypes GetCurrentAbilityType() const;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Type", Replicated)
	TObjectPtr<UBaseAbilityType> CurrentAbilityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Type")
	TArray<TSubclassOf<UBaseAbilityType>> ValidAbilityTypeClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Type")
	TMap<ESmashAbilityTypes, TObjectPtr<UBaseAbilityType>> ValidAbilityTypePtrs;
};
