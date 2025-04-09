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
	USmashCharacterStats();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Smash Character Stats")
	void Screenshake(float RumbleInten, float RumbleDuration);

protected:
	UFUNCTION()
	void OnPercentChanged(int32 OldPercent, int32 NewPercent);

public:
	// 피격게이지 (실제 값은 SmashCombatComponent에서 관리됨)
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Percent;

	// 스톡 (남은 목숨)
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
