#include "Character/Components/SmashCombatComponent.h"
#include "Character/SmashCharacter.h"
#include "Character/Components/SmashCharacterStats.h"
#include "Character/Components/SmashStateSystem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Character/Components/FXComponent.h"
#include "Kismet/GameplayStatics.h"

USmashCombatComponent::USmashCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USmashCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 필요한 변수를 DOREPLIFETIME으로 추가 가능
}

void USmashCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ASmashCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("SmashCombatComponent: 소유자는 ASmashCharacter가 아닙니다!"));
		return;
	}

	CharacterStats = OwnerCharacter->SmashCharacterStatsComponent;
	if (!CharacterStats)
	{
		UE_LOG(LogTemp, Error, TEXT("SmashCombatComponent: 캐릭터 스탯 컴포넌트를 찾을 수 없습니다!"));
	}
}

void USmashCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void USmashCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 넉백 물리나 추가 로직이 있다면 여기서 처리
}

void USmashCombatComponent::TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection)
{
	// 클라이언트이면 서버 RPC 호출
	if (!GetOwner()->HasAuthority())
	{
		Server_TakeDamage(DamageAmount, AttackType, bIsRightDirection);
		return;
	}

	if (!CharacterStats || !OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("SmashCombatComponent: TakeDamage - Stats 또는 OwnerCharacter가 없습니다."));
		return;
	}

	// 무적이면 무시
	if (CharacterStats->IsInvulnerable())
	{
		return;
	}

	// 1) 데미지 누적 (피격게이지 증가)
	CharacterStats->AddPercent(DamageAmount);

	// 2) 공격 유형에 따른 처리
	if (AttackType == ESmashAttackType::Knockback)
	{
		// 넉백
		ApplyKnockback(bIsRightDirection);

		// Tumble 등 상태 변경
		if (OwnerCharacter->SmashStateSystem)
		{
			OwnerCharacter->SmashStateSystem->TryChangeState(ESmashPlayerStates::Tumble);
		}
	}
	else
	{
		// Normal Attack (피격게이지만 증가)
		if (OwnerCharacter->SmashStateSystem)
		{
			ESmashPlayerStates CurrentState = OwnerCharacter->SmashStateSystem->GetCurrentState();
			// if (  CurrentState != ESmashPlayerStates::Dizzy && CurrentState != ESmashPlayerStates::Tumble)
			// {
				OwnerCharacter->SmashStateSystem->TryChangeState(ESmashPlayerStates::Hit);
			// }
		}
	}

	// 3) 피격 후 잠시 무적
	CharacterStats->SetInvulnerable(true, HitInvulnerabilityDuration);

	// 4) 이펙트/사운드
	if (OwnerCharacter->FXComponent)
	{
		OwnerCharacter->FXComponent->PlayHitEffect(OwnerCharacter->GetActorLocation());
	}
}

void USmashCombatComponent::Server_TakeDamage_Implementation(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection)
{
	TakeDamage(DamageAmount, AttackType, bIsRightDirection);
}

void USmashCombatComponent::ApplyKnockback(bool bIsRightDirection)
{
	// 필수 컴포넌트 유효성 검사
	if (!OwnerCharacter || !CharacterStats)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyKnockback: 유효하지 않은 OwnerCharacter 또는 CharacterStats"));
		return;
	}

	// 현재 피격 게이지 취득
	float CurrentPercent = CharacterStats->GetPercent();

	// 최소 기본 넉백 보장 (최소 넉백과 현재 피격 게이지에 따른 넉백 중 큰 값 선택)
	float KnockbackMagnitude = FMath::Max(MinBaseKnockback, BaseKnockback + (CurrentPercent * KnockbackGrowthFactor));

	// 캐릭터 무게에 따른 넉백 조정
	float WeightFactor = 100.0f / FMath::Max(CharacterStats->Weight, 1.0f); // 0으로 나누기 방지
	KnockbackMagnitude *= WeightFactor;

	// 최대 넉백 강도로 제한
	KnockbackMagnitude = FMath::Min(KnockbackMagnitude, MaxKnockbackMagnitude);

	// 방향에 따라 X 방향 결정 (오른쪽 = 양수, 왼쪽 = 음수)
	float DirectionMultiplier = bIsRightDirection ? 1.0f : -1.0f;

    // Z축 상승 계산 수정 - 균형잡힌 범위 사용 및 비선형 커브 적용
    // 피격 게이지가 낮을 때는 충분한 Z축 상승을 보장하고, 높아질수록 증가율을 감소시킴
    float PercentRatio = FMath::Clamp(CurrentPercent / 1000.0f, 0.0f, 1.0f); // 150%를 최대값으로 고려
    
    // 초기값은 더 높게 시작하되 증가율은 낮추기 위해 제곱근 대신 다른 함수 사용
    // (1 - (1 - x)²) 함수는 낮은 값에서는 빠르게 증가하다가 높은 값에서는 완만하게 증가
	float NonLinearFactor = PercentRatio;
    //float NonLinearFactor = 1.0f - FMath::Square(1.0f - PercentRatio);
    
    // 시작값을 0.35로 높이고 최대값을 0.6으로 제한
    float ZAxisFactor = FMath::Lerp(0.35f, 0.6f, NonLinearFactor);

    // 피격 게이지가 높을수록 X축(수평) 방향 넉백 강화
    float XAxisMultiplier = 1.0f + (2.0f * PercentRatio); // 1.0~1.5 범위로 X축 넉백 강화

	// 약간의 무작위성 추가 (자연스러운 넉백 구현)
	// 0.9~1.1 사이의 랜덤 변동 적용
	float RandomVariation = FMath::RandRange(0.9f, 1.1f);

	// 최종 넉백 벡터 계산
	FVector LaunchVelocity(
        KnockbackMagnitude * DirectionMultiplier * RandomVariation * XAxisMultiplier * 1.7f, // X 방향 (좌/우) - 강화됨
		0.0f, // Y 방향 (사용하지 않음)
        KnockbackMagnitude * ZAxisFactor * RandomVariation * 4.0f // Z 방향 (수직) - 조정됨
	);

	// 캐릭터 발사 (X,Y 속도는 덮어쓰기, Z 속도도 덮어쓰기)
	LaunchCharacter(LaunchVelocity, true, true);
}


void USmashCombatComponent::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (OwnerCharacter && IsValid(OwnerCharacter))
	{
		OwnerCharacter->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	}
}

void USmashCombatComponent::Server_LaunchCharacter_Implementation(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}
