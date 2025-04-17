// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobSpawnerPhase2.generated.h"

UCLASS()
class SMASHBRAWL_API AMobSpawnerPhase2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMobSpawnerPhase2();

	FVector GetSpanwerVolume() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* SpawnVolumeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	class TSubclassOf<class ABaseAIFighter> AIMonster;

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
};