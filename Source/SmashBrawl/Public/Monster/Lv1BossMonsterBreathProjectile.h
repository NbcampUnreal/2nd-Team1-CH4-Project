// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lv1BossMonsterBreathProjectile.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonsterBreathProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1BossMonsterBreathProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage;

protected:
	UFUNCTION()
	void OnProjectileOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);
public:
	
	FORCEINLINE float GetDamage() const { return Damage; }

	FORCEINLINE void SetDamage(const float Value) { Damage = Value; }

	virtual void Destroyed() override;
	
};
