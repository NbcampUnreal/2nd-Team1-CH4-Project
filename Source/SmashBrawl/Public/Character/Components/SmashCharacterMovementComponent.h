// 

#pragma once

#include "CoreMinimal.h"
#include "SSTCharacterMovementComponent.h"
#include "SmashCharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashCharacterMovementComponent : public USSTCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterMovementComponent();

	// 네트워크 복제 속성 등록 함수 추가
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// RequestTurnAround 함수 오버라이드
	virtual bool RequestTurnAround() override;

	// bCanFlip이 변경될 때 호출될 함수
	UFUNCTION()
	void OnRep_bCanFlip();

	// bCanFlip을 Replicated로 표시하고 OnRep 함수 연결
	UPROPERTY(ReplicatedUsing=OnRep_bCanFlip, BlueprintReadWrite, Category = "Smash Character Movement Component")
	bool bCanFlip;
};
