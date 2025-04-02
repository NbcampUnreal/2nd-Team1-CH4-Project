
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAbility.generated.h"


UCLASS()
class SMASHBRAWL_API ABaseAbility : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EndAbility()
	{
	}


	UPROPERTY()
	bool bActive;

	UPROPERTY()
	TObjectPtr<ACharacter> Parent;

	UPROPERTY()
	float ChargeLevel;
};
