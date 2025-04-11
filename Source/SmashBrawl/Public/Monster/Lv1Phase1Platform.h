// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lv1Phase1Platform.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1Phase1Platform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1Phase1Platform();

	virtual void BeginPlay() override;

	UFUNCTION()
	void Destruction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxComp;
};
