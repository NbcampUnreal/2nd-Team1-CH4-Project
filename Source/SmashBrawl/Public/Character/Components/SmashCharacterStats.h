// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterStats.generated.h"


class ASmashCharacter;
class USmashGameInstance;

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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(int32 NewDamage);

	void Screenshake(float RumbleInten,float RumbleDuration);

public:
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 HP;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Percent;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 HPMAX;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Stock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float DefaultGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 SuperIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Elims;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 DamageDone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 DamageTaken;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 SD;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Falls;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character Stats")
	TObjectPtr<ASmashCharacter> Parent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats|Config")
	TSubclassOf<class UCameraShakeBase> ShakeClass;
};
