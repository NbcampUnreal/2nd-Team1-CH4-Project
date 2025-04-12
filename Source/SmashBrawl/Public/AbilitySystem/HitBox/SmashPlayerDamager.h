// 

#pragma once

#include "CoreMinimal.h"
#include "SmashBaseDamager.h"
#include "Interfaces/Interface_SmashHitBox.h"
#include "SmashPlayerDamager.generated.h"

UCLASS()
class SMASHBRAWL_API ASmashPlayerDamager : public ASmashBaseDamager, public IInterface_SmashHitBoxPriority
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmashPlayerDamager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	virtual const int32 GetPriority() override;
	
	UFUNCTION()
	virtual const int32 ComparatorPriority(int32 OtherPriority) override;
	
	UFUNCTION()
	virtual void ActionPriority(int32 OtherPriority) override;
	
	UFUNCTION()
	void Flinch();

	virtual void AttackActor(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		) override;

	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	int32 Priority;
};
