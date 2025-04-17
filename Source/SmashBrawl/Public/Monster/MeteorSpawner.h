// 

#pragma once

#include "CoreMinimal.h"
#include "Lv1BossMonsterMeteor.h"
#include "AbilitySystem/HitBox/SmashMonsterDamagerManager.h"
#include "GameFramework/Actor.h"
#include "Core/SmashTypes.h"
#include "MeteorSpawner.generated.h"

UCLASS()
class SMASHBRAWL_API AMeteorSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeteorSpawner();

	FVector GetSpanwerVolume() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* SpawnVolumeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	class TSubclassOf<ALv1BossMonsterMeteor> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	ESmashSpawnerDirection SpawnDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	class ABaseBossMonster* BossMonster;

	UFUNCTION()
	void StartActive();

	UFUNCTION()
	void StopActive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMeteor() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnTime = 0.5f;

	UPROPERTY()
	FTimerHandle MeteorSpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USmashMonsterDamagerManager* DamagerManager;
};
