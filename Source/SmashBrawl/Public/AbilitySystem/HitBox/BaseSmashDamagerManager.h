// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSmashDamagerManager.generated.h"


class ASmashBaseDamager;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UBaseSmashDamagerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseSmashDamagerManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	AActor* CreateDamager(FTransform SpawnTransform, const FName& DamagerName);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	UDataTable* HitBoxMappingTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	UDataTable* DamagePlayTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	UDataTable* DamageVisualTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	TArray<TSubclassOf<AActor>> AttackAbleClasses;
	
};
