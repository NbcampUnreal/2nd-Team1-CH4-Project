#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "SmashCharacterStats.generated.h"

class ASmashCharacter;
class USmashGameInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashCharacterStats();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 스크린 쉐이크 효과
	UFUNCTION(BlueprintCallable, Category = "Smash Character Stats")
	void Screenshake(float RumbleInten, float RumbleDuration);

	// 피격게이지 설정/획득 함수
	UFUNCTION(BlueprintCallable, Category = "Smash Character Stats")
	void SetPercent(int32 NewPercent);

	UFUNCTION(BlueprintCallable, Category = "Smash Character Stats")
	void AddPercent(int32 PercentToAdd);

	UFUNCTION(BlueprintPure, Category = "Smash Character Stats")
	float GetPercent() const { return Percent; }

	// SuperIndex 설정/획득 함수
	UFUNCTION(BlueprintCallable, Category = "Smash Character Stats")
	void SetSuper(int32 NewSuper);

	UFUNCTION(BlueprintPure, Category = "Smash Character Stats")
	int32 GetSuper() const { return SuperIndex; }

	// 무적 상태 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Smash Character Stats")
	void SetInvulnerable(bool bNewInvulnerable, float Duration = 0.0f);

	UFUNCTION(BlueprintPure, Category = "Smash Character Stats")
	bool IsInvulnerable() const { return bIsInvulnerable; }

protected:
	// 피격게이지 복제 처리 함수
	UFUNCTION()
	void OnRep_Percent();

	// 무적 상태 복제 처리 함수
	UFUNCTION()
	void OnRep_IsInvulnerable();

	// 자연 회복 로직을 처리하는 함수
	UFUNCTION()
	void ProcessNaturalRecovery();
	
	// 자연 회복 타이머 관리 함수
	void UpdateNaturalRecoveryTimer();

public:
	// 피격게이지 (실제 값은 이 컴포넌트에서 관리)
	UPROPERTY(ReplicatedUsing = OnRep_Percent, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float Percent;

	// 피격게이지 최대값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats|Config")
	float MaxPercent;

	// 스톡 (남은 목숨)
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Stock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float DefaultGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 SuperIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Elims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 DamageDone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 DamageTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 SD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats")
	int32 Falls;

	// 무적 상태 여부
	UPROPERTY(ReplicatedUsing = OnRep_IsInvulnerable, BlueprintReadOnly, Category = "Smash Character Stats")
	bool bIsInvulnerable;

	// 자연 회복 활성화 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats|Config")
	bool bEnableNaturalRecovery;
	
	// 자연 회복 속도 (초당 감소량)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats|Config")
	int32 NaturalRecoveryRate;
	
	// 자연 회복 타이머 간격 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats|Config")
	float NaturalRecoveryInterval;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character Stats")
	TObjectPtr<ASmashCharacter> Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character Stats|Config")
	TSubclassOf<class UCameraShakeBase> ShakeClass;

	// 타이머 핸들
	FTimerHandle InvulnerabilityTimerHandle;

	// 자연 회복 타이머 핸들
	FTimerHandle NaturalRecoveryTimerHandle;

public:
	// 피격게이지 변경 이벤트 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPercentChangedSignature, int32, OldPercent, int32, NewPercent);

	// 사망 이벤트 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

	UPROPERTY(BlueprintAssignable, Category = "Smash Character Stats|Delegate Events")
	FOnPercentChangedSignature OnPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "Smash Character Stats|Delegate Events")
	FOnDeathSignature OnDeath;
};