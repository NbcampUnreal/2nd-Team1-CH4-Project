#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "SmashCombatComponent.generated.h"

// 전방 선언
class ASmashCharacter;


// 피격게이지 변경 이벤트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPercentChangedSignature, int32, OldPercent, int32, NewPercent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashCombatComponent();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 데미지 처리 함수 - 넉백 방향을 bool로 변경
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection = true, float KnockbackMultiplier = 1.0f);

	// 서버 RPC로 데미지 처리 요청 - 넉백 방향을 bool로 변경
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void Server_TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection = true, float KnockbackMultiplier = 1.0f);

	// 넉백 계산 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float CalculateKnockback(float InDamageScale, float InWeight, float InBaseKnock, float InHitScale, float InDamRatios = 1.0f);

	// 피격게이지 설정/획득 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetPercent(int32 NewPercent);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddPercent(int32 PercentToAdd);

	UFUNCTION(BlueprintPure, Category = "Combat")
	int32 GetPercent() const { return Percent; }

	// 장외 체크 및 처리 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void CheckOutOfBounds();

	// 피격 이펙트 재생 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PlayHitEffect(bool bIsKnockback);

	// 캐릭터 런치 함수 (새로 추가)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

	// 서버 런치 함수 (새로 추가)
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void Server_LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

	// 넉백 적용 함수 - 넉백 방향을 bool로 변경
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyKnockback(bool bIsRightDirection, float KnockbackForce);

	// 피격 상태 효과 업데이트 (SmashCharacter에서 이동)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UpdateHitEffect();

	// 피격게이지 변경 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnPercentChangedSignature OnPercentChanged;

	// 사망 이벤트 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnDeathSignature OnDeath;

protected:
	// 피격게이지 (복제됨)
	UPROPERTY(ReplicatedUsing = OnRep_Percent, BlueprintReadOnly, Category = "Combat")
	int32 Percent;

	// 피격게이지 최대값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	int32 MaxPercent;

	// 맵 경계 값 - 구성 가능하게 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	FVector MapBounds;

	// 피격 후 무적 시간 - 구성 가능하게 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float HitInvulnerabilityDuration;

	// 넉백 보정 값 - 구성 가능하게 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float MinVerticalMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float MaxVerticalMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float MinAirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float MaxAirControl;

	// 넉백 계산 상수 - 구성 가능하게 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float KnockbackBaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float KnockbackWeightFactor;

	// 피격게이지 복제 처리 함수
	UFUNCTION()
	void OnRep_Percent();

	// 피격 이펙트 타이머 관리
	FTimerHandle HitEffectTimerHandle;

	// 소유 캐릭터 참조
	UPROPERTY()
	TObjectPtr<ASmashCharacter> OwnerCharacter;

	// 무적 상태 여부
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsInvulnerable;

	// 무적 상태 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInvulnerable(bool bNewInvulnerable, float Duration = 0.0f);

	// 장외 체크 최적화를 위한 변수
	float LastOutOfBoundsCheckTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	float OutOfBoundsCheckInterval;

public:
	// 사용자 정의 값 배열 (예: 데미지 스케일)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	TArray<float> DamageScale;

	// 현재 사용된 이동 카운트
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Combat")
	int32 UsedMovesCount;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	int32 BaseKnock = 5;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	float HitScale;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	float DamRatios = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	int32 Damage;

	// SmashCharacter에서 이동된 변수들
	UPROPERTY(BlueprintReadWrite, Category = "Combat|Effects")
	float HitFlash;
};
