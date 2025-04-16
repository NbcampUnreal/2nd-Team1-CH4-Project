// 

#pragma once

#include "CoreMinimal.h"
#include "Core/DamageTable.h"
#include "GameFramework/Actor.h"
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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
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
	virtual bool bIsAttackAble(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION(Server, Reliable)
	virtual void AttackActor(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void LifeTimeOut();

	void Init(const TObjectPtr<AActor> InParent,
		const TArray<TSubclassOf<AActor>>& InAttackAbleClasses,
		const FDamagePlayRow& InDamagePlayRow,
		const FDamageVisualRow& InDamageVisualRow = FDamageVisualRow());

	UFUNCTION()
	void DetectOverlapActor();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayHitadditional();
	UFUNCTION(Server, Reliable)
	void Server_PlayHitadditional();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility Hitbox")
	TObjectPtr<UStaticMeshComponent> SmashDamageBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<AActor> Parent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TSet<TObjectPtr<AActor>> IgnoreActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	TArray<TSubclassOf<AActor>> AttackAbleClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	FDamagePlayRow DamagePlayRow;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	FDamageVisualRow DamageVisualRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility DamageTable")
	class USceneComponent* RealRootComponent;;
	
	FTimerHandle LifeTimer;
};

