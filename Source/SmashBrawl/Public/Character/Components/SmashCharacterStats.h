// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterStats.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(int32 NewDamage)
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float DefaultGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int SuperIndex;
};
