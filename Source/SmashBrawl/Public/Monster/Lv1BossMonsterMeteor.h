// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Lv1BossMonsterMeteor.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonsterMeteor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALv1BossMonsterMeteor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* NiagaraTrailEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffectTemplate;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraDestroyEffectTemplate;

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