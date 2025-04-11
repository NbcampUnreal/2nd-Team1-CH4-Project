// 

#pragma once

#include "CoreMinimal.h"
#include "Core/SmashDamageBoxType.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	int32 DamagerIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FDamagerInfoProperty DamagerInfoProperty;
};
