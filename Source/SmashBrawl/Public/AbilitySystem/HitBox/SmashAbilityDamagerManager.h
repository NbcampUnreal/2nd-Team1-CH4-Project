// 

#pragma once

#include "CoreMinimal.h"
#include "BaseSmashDamagerManager.h"
#include "Core/DamageTable.h"
#include "Core/SmashDamageBoxType.h"
#include "SmashAbilityDamagerManager.generated.h"


class ASmashCharacter;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashAbilityDamagerManager : public UBaseSmashDamagerManager
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashAbilityDamagerManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	TArray<TObjectPtr<AActor>> SpawnDamagerAll();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityDamagerManager")
	TArray<FSmashDamagerInfoProperty> DamagerInfos;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityDamagerManager")
	TObjectPtr<ASmashCharacter> Parent;
};
