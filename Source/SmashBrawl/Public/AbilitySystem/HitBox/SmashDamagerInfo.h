// 

#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "GameFramework/Actor.h"
#include "SmashDamagerInfo.generated.h"

UCLASS()
class SMASHBRAWL_API ASmashDamagerInfo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmashDamagerInfo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetSmashMesh(UStaticMeshComponent* StaticMeshComponent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SmashAttackRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SmashScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SmashRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> Particle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHitDirection HitDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	float LifeTime;
};
