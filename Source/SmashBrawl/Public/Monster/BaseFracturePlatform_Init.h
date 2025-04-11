// 

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "BaseFracturePlatform_Init.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseFracturePlatform_Init : public AGeometryCollectionActor
{
	GENERATED_BODY()
	
public:
	ABaseFracturePlatform_Init();

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Fracture")
	void MultiCast_Destruction();

	UFUNCTION()
	void DestroyPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	class ABaseBossMonster* BossMonster;
	
	FTimerHandle PlatformDestroyTimerHandle;
};