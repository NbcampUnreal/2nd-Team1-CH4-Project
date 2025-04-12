// 

#pragma once

#include "CoreMinimal.h"
#include "SmashTypes.h"
#include "DamageTable.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct SMASHBRAWL_API FDamagePlayRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DamagePlayRowId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESmashAttackType AttackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHitDirection HitDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> DamagerVisualActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	float LifeTime;
};

USTRUCT(BlueprintType)
struct SMASHBRAWL_API FDamageVisualRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitboxVisualRowId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> Particle;
	
};

USTRUCT(BlueprintType)
struct SMASHBRAWL_API FHitBoxMappingRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DamagerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DamagePlayRowId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitboxVisualRowId;
};
