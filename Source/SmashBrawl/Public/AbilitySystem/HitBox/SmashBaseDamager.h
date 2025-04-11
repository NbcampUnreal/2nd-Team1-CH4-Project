// 

#pragma once

#include "CoreMinimal.h"
#include "Core/SmashDamageBoxType.h"
#include "Core/SmashTypes.h"
#include "GameFramework/Actor.h"
#include "SmashBaseDamager.generated.h"

/**
 * BP_Damager및 Grab Box 스폰을 위해
 */
UCLASS()
class SMASHBRAWL_API ASmashBaseDamager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmashBaseDamager();

	UFUNCTION(BlueprintCallable, Category="SmashAbility Hitbox")
	void Init(ACharacter* Parent, const FHitProperty HitProperty);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 블루프린트에서 생성과 클래스에서 생성을 나눠 true인경우 블루프린트에서 사용하는 변수에 여기변수를 적용한다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	bool bSpawnByClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<AActor> SpawningOwner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<ACharacter> SpawningParent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	float SpawningLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FHitProperty SpawningHitProperty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	int32 SpawningPriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	ESmashFaceing SpawningDirection;
};
