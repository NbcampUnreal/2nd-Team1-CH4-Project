// 

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "BaseFracturePlatform_Phase2.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseFracturePlatform_Phase2 : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	ABaseFracturePlatform_Phase2();

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Fracture")
	void MultiCast_Destruction();

	UFUNCTION()
	void DestroyPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	class ABaseBossMonster* BossMonster;
	
	FTimerHandle PlatformDestroyTimerHandle;
};