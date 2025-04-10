// 

#pragma once

#include "CoreMinimal.h"
#include "Core/SmashDamageBoxType.h"
#include "Core/SmashTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_SmashHitBox.h"
#include "SmashBaseDamager.generated.h"

class ASmashCharacter;
/**
 * BP_Damager및 Grab Box 스폰을 위해
 */
UCLASS()
class SMASHBRAWL_API ASmashBaseDamager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmashBaseDamager();

	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void Init(AActor* InParent, const FHitProperty InHitProperty);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void OnMeshBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void LifeTimeOut();
public:
	UPROPERTY(EditAnywhere)
	TArray<TScriptInterface<IInterface_SmashHitBox>> SmashHitModels;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility Hitbox")
	TObjectPtr<UStaticMeshComponent> SmashDamageBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<UStaticMesh> HitBoxMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TArray<TScriptInterface<IInterface_SmashHitBox>> HitModels;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<AActor> Parent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	float LifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FHitProperty HitProperty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	bool bIsRightDirection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TSet<AActor*> IgnoreActors;

	FTimerHandle LifeTimer;
};

