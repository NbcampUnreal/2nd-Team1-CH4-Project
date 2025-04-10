#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FXComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFXComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void FXMainLoop();

	UFUNCTION(BlueprintCallable, Category = "FX|Effects")
	void PlayHitEffect(FVector Location);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX|Effects")
	UParticleSystem* HitParticleSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX|Effects")
	USoundBase* HitSound = nullptr; // 적절한 사운드 에셋 참조 필요
};
