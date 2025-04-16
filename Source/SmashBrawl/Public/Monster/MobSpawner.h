// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobSpawner.generated.h"

UCLASS()
class SMASHBRAWL_API AMobSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMobSpawner();

	FVector GetSpanwerVolume() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* SpawnVolumeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	class TSubclassOf<class ABaseAIFighter> AIMonster;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMeteor() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnTime = 0.5f;

	UPROPERTY()
	FTimerHandle MeteorSpawnTimer;
};