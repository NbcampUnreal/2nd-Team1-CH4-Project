#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "SmashCombatComponent.generated.h"

class ASmashCharacter;
class USmashCharacterStats;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashCombatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 
	 * TakeDamage
	 * @param DamageAmount		 : 추가될 데미지 양 (피격게이지 증가치)
	 * @param AttackType         : 공격 유형 (넉백/일반)
	 * @param bIsRightDirection  : 맞은 방향이 오른쪽에서 왔는지 (true=오른쪽 / false=왼쪽)
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection = true);

	/** 서버용 RPC: TakeDamage를 호출하여 서버에서 처리 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void Server_TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection = true);

	/**
	 * ApplyKnockback
	 * @param bIsRightDirection  : 넉백 방향 (true = +X, false = -X)
	 * @param KnockbackMultiplier: 넉백 강도 배율 (특수 상황이 아니라면 1.0f)
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyKnockback(bool bIsRightDirection);

	/**
	 * 실제 캐릭터를 런치(발사)합니다.
	 * @param LaunchVelocity: 적용할 런치 벡터(XZ)
	 * @param bXYOverride   : X/Y 기존속도 무시 여부
	 * @param bZOverride    : Z 기존속도 무시 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

	/** 서버 RPC: LaunchCharacter를 서버에서 호출 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void Server_LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

protected:
	/** 소유 캐릭터 참조 */
	UPROPERTY(BlueprintReadWrite, Category="Combat|Reference")
	TObjectPtr<ASmashCharacter> OwnerCharacter;

	/** 캐릭터 스탯 컴포넌트 (피격게이지, 무적 등) */
	UPROPERTY(BlueprintReadWrite, Category="Combat|Reference")
	TObjectPtr<USmashCharacterStats> CharacterStats;

	/**  
	 * 피격 후 잠시 무적이 되는 시간(초).
	 * 예: 0.5f -> 0.5초 동안 무적.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Config")
	float HitInvulnerabilityDuration = 0.5f;


	// 최소 기본 넉백 강도 (아주 낮은 피격 게이지에서도 의미 있는 넉백 보장)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Config|KnockBack")
	float MinBaseKnockback = 300.0f;
	
	// 기본 넉백 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Config|KnockBack")
	float BaseKnockback = 500.0f;

	// 피격 게이지에 따른 넉백 증가율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Config|KnockBack")
	float KnockbackGrowthFactor = 3.0f;

	// 최대 넉백 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Config|KnockBack")
	float MaxKnockbackMagnitude = 2500.0f;
};
