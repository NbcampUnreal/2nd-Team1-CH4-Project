// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashDamageBoxType.h"
#include "SmashAbilityHitboxComponent.generated.h"

class ASmashBaseDamager;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashAbilityHitboxComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashAbilityHitboxComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void SpawnDamageBox(int32 CollisionSet);
	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void SpawnHitBox(FHitProperty HitProperty);
	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void SpawnGrabBox(FHitProperty HitProperty);

	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void SetBoxTransform(int32 CollisionSet);

	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void AddHitProperty(FHitProperty HitProperties);
	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void FixHitProperty(int32 CollisionSet, FHitProperty HitProperties);
	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	FHitProperty GetHitProperty(int32 CollisionSet);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility HitBoxSystem|References")
	TObjectPtr<ACharacter> Parent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SmashAbility HitBoxSystem|References")
	TArray<FHitProperty> HitProperties;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SmashAbility HitBoxSystem|References")
	TSubclassOf<ASmashBaseDamager> SpawnHitClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SmashAbility HitBoxSystem|References")
	TSubclassOf<ASmashBaseDamager> SpawnGrabClass;
};
