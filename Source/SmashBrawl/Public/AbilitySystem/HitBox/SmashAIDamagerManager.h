// 

#pragma once

#include "CoreMinimal.h"
#include "BaseSmashDamagerManager.h"
#include "SmashAIDamagerManager.generated.h"


struct FSmashDamagerInfoProperty;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashAIDamagerManager : public UBaseSmashDamagerManager
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashAIDamagerManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	TArray<TObjectPtr<AActor>> SpawnDamagerAll();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityDamagerManager")
	TArray<FSmashDamagerInfoProperty> DamagerInfos;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityDamagerManager")
	TObjectPtr<AActor> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityDamagerManager")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDamagerManager")
	FName SocketName;
};
