// 

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "BaseFracturePlatform.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseFracturePlatform : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	ABaseFracturePlatform();

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Fracture")
	void MultiCast_Destruction();

	UFUNCTION()
	void DestroyPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	class ABaseBossMonster* BossMonster;
	
	FTimerHandle PlatformDestroyTimerHandle;
};