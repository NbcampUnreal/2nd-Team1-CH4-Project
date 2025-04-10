// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashDamageBoxType.generated.h"

/**
 * 데미지 박스
 */
UENUM(BlueprintType)
enum class FSmashCollisionType : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Sweet UMETA(DisplayName = "Sweet"),
	Sour UMETA(DisplayName = "Sour"),
	Spike UMETA(DisplayName = "Spike"),
	Meteor UMETA(DisplayName = "Meteor"),
	Multi UMETA(DisplayName = "Multi")
};

UENUM(BlueprintType)
enum class FSmashCollisionShape : uint8
{
	Sphere UMETA(DisplayName = "Sphere"),
	Box UMETA(DisplayName = "Box"),
	Pill UMETA(DisplayName = "Pill")
};

USTRUCT(BlueprintType)
struct FHitProperty
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FName Attacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FSmashCollisionType SmashCollisionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FSmashCollisionShape SmashCollisionShape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FName CollisionSocket;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<USoundBase> Sound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TObjectPtr<UParticleSystem> HitParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	bool GrabBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FTransform BoxTransform;
};

USTRUCT(BlueprintType)
struct FDamagerInfoProperty
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FName Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TSubclassOf<UStaticMesh> StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TSubclassOf<USoundBase> Sound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	TSubclassOf<UParticleSystem> HitParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility Hitbox")
	FTransform BoxTransform;
};