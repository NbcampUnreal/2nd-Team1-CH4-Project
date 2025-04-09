#include "Character/Components/SmashCombatComponent.h"
#include "Character/SmashCharacter.h"
#include "Character/Components/SmashStateSystem.h"
#include "Character/Components/SmashCharacterStats.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Core/SmashGameInstance.h"
#include "Widgets/HUD/InGame/UW_HUD_CharacterInfo.h"

USmashCombatComponent::USmashCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	// 기본값 설정
	Percent = 0;
	MaxPercent = 999;
	bIsInvulnerable = false;
	UsedMovesCount = 0;
	HitFlash = 0.0f;

	// 맵 경계 기본값 설정
	MapBounds = FVector(5000.0f, 5000.0f, 3000.0f);

	// 무적 시간 기본값 설정
	HitInvulnerabilityDuration = 0.5f;

	// 넉백 보정 값 기본값 설정
	MinVerticalMultiplier = 0.5f;
	MaxVerticalMultiplier = 1.5f;
	MinAirControl = 0.2f;
	MaxAirControl = 1.0f;

	// 넉백 계산 상수 기본값 설정
	KnockbackBaseValue = 18.0f;
	KnockbackWeightFactor = 1.4f;

	// 기본 데미지 스케일 설정
	DamageScale.Add(1.0f); // 기본 스케일

	// Tick 최적화를 위한 변수 추가
	LastOutOfBoundsCheckTime = 0.0f;
	OutOfBoundsCheckInterval = 0.1f; // 100ms마다 장외 체크
}

void USmashCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 속성 등록 - 조건부 복제로 최적화
	DOREPLIFETIME(USmashCombatComponent, Percent);
	DOREPLIFETIME_CONDITION(USmashCombatComponent, bIsInvulnerable, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USmashCombatComponent, UsedMovesCount, COND_OwnerOnly);
}

void USmashCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유 캐릭터 참조 설정
	OwnerCharacter = Cast<ASmashCharacter>(GetOwner());

	// SmashCharacterStats와 연결
	if (OwnerCharacter && OwnerCharacter->SmashCharacterStatsComponent)
	{
		// 초기 Percent 동기화
		OwnerCharacter->SmashCharacterStatsComponent->Percent = Percent;
	}
}

void USmashCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 모든 타이머 정리
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HitEffectTimerHandle);
	}
}

void USmashCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 장외 체크 (서버에서만 실행) - 매 틱이 아닌 일정 간격으로 수행
	if (GetOwner()->HasAuthority())
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastOutOfBoundsCheckTime >= OutOfBoundsCheckInterval)
		{
			CheckOutOfBounds();
			LastOutOfBoundsCheckTime = CurrentTime;
		}
	}
}

// 데미지 처리 함수 - 넉백 방향을 bool로 변경
void USmashCombatComponent::TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection, float KnockbackMultiplier)
{
	// 클라이언트에서는 서버에 요청
	if (!GetOwner()->HasAuthority())
	{
		Server_TakeDamage(DamageAmount, AttackType, bIsRightDirection, KnockbackMultiplier);
		return;
	}

	// 무적 상태 체크
	if (bIsInvulnerable)
	{
		return;
	}

	// 데미지 처리
	int32 OldPercent = Percent;
	Percent = FMath::Clamp(Percent + DamageAmount, 0, MaxPercent);

	// 피격게이지 변경 이벤트 호출
	OnPercentChanged.Broadcast(OldPercent, Percent);

	// SmashCharacterStats에도 업데이트 (null 체크 추가)
	if (OwnerCharacter && OwnerCharacter->SmashCharacterStatsComponent)
	{
		OwnerCharacter->SmashCharacterStatsComponent->Percent = Percent;
		OwnerCharacter->SmashCharacterStatsComponent->DamageTaken += DamageAmount;

		// SuperIndex 업데이트 (필요한 경우 여기서 조정)
		OwnerCharacter->SmashCharacterStatsComponent->SuperIndex += DamageAmount * 0.5f;
	}

	// 이펙트 재생
	PlayHitEffect(AttackType == ESmashAttackType::Knockback);

	// 넉백 유형 공격인 경우
	if (AttackType == ESmashAttackType::Knockback && OwnerCharacter)
	{
		// BaseKnock와 HitScale, DamRatios 체크 및 기본값 설정
		float CalBaseKnock = BaseKnock > 0.0f ? BaseKnock : 1.0f;
		float CalHitScale = HitScale > 0.0f ? HitScale : 1.0f;
		float CalDamRatios = DamRatios > 0.0f ? DamRatios : 1.0f;

		// 무게 값 가져오기 (null 체크 추가)
		float Weight = 100.0f;
		if (OwnerCharacter->SmashCharacterStatsComponent)
		{
			Weight = OwnerCharacter->SmashCharacterStatsComponent->Weight;
		}

		// 넉백 계산 및 적용
		float KnockbackForce = CalculateKnockback(
			DamageScale.IsValidIndex(UsedMovesCount) ? DamageScale[UsedMovesCount] : 1.0f,
			Weight,
			CalBaseKnock,
			CalHitScale,
			CalDamRatios
		);

		// 최종 넉백 값 계산
		KnockbackForce *= KnockbackMultiplier;

		// 넉백 적용 (방향을 bool로 변경)
		ApplyKnockback(bIsRightDirection, KnockbackForce);

		// 캐릭터 상태 변경 (히트 상태로)
		if (OwnerCharacter->SmashStateSystem)
		{
			OwnerCharacter->SmashStateSystem->TryChangeState(ESmashPlayerStates::Hit);
		}
	}

	// 피격 후 짧은 무적 시간 부여
	SetInvulnerable(true, HitInvulnerabilityDuration);
}

// 서버 RPC로 데미지 처리 요청 - 넉백 방향을 bool로 변경
void USmashCombatComponent::Server_TakeDamage_Implementation(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection, float KnockbackMultiplier)
{
	TakeDamage(DamageAmount, AttackType, bIsRightDirection, KnockbackMultiplier);
}

float USmashCombatComponent::CalculateKnockback(float InDamageScale, float InWeight, float InBaseKnock, float InHitScale, float InDamRatios)
{
	if (!OwnerCharacter || !IsValid(OwnerCharacter))
	{
		return InBaseKnock;
	}

	// Damage 변수 명확화 - 기본값 사용
	float CharacterDamage = 1.0f;

	// 넉백 계산 공식 - 주석 추가
	float DamageScaleFactor = InDamageScale; // 데미지 스케일
	float PercentFactor = Percent / 10.0f; // 기본 피격게이지 계수
	float WeightedDamageFactor = (Percent * CharacterDamage) / 20.f; // 데미지 가중치 계수

	// 1단계: 기본 데미지 계산
	float BaseDamage = DamageScaleFactor * (PercentFactor + WeightedDamageFactor);

	// 2단계: 무게 기반 보정 계산
	float WeightFactor = (200.0f / (InWeight + 100.0f)) * KnockbackWeightFactor; // 무게에 따른 보정

	// 3단계: 기본 데미지와 무게 보정 조합
	float CombinedDamage = (BaseDamage * WeightFactor) + KnockbackBaseValue;

	// 4단계: 피격게이지와 히트 스케일 기반 최종 보정
	float PercentScaleFactor = (Percent / 7.0f) * InHitScale;

	// 최종 넉백 계산
	float FinalKnockback = (CombinedDamage * PercentScaleFactor) + InBaseKnock;

	// 타격비율 적용
	return FinalKnockback * InDamRatios;
}

// 넉백 적용 함수 - 넉백 방향을 bool로 변경
void USmashCombatComponent::ApplyKnockback(bool bIsRightDirection, float KnockbackForce)
{
	if (!OwnerCharacter || !IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("USmashCombatComponent::ApplyKnockback - OwnerCharacter is invalid"));
		return;
	}

	// 캐릭터 무브먼트 컴포넌트 체크
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	// 넉백 방향 설정 (bool 값에 따라 방향 결정)
	FVector KnockbackDirection = FVector::RightVector; // 기본 오른쪽 방향
	if (!bIsRightDirection)
	{
		KnockbackDirection = FVector::LeftVector; // 왼쪽 방향
	}

	// 넉백 벡터 계산
	FVector KnockbackVector = KnockbackDirection * KnockbackForce;

	// 피격게이지가 높을수록 더 높게 날아가도록 수직 방향 보정
	float VerticalMultiplier = FMath::Lerp(MinVerticalMultiplier, MaxVerticalMultiplier, Percent / static_cast<float>(MaxPercent));
	KnockbackVector.Z = KnockbackForce * VerticalMultiplier; // 항상 위로 날아가도록 양수 값 설정

	// 피격게이지가 높을수록 공중에서 컨트롤이 제한되도록 설정
	float AirControl = FMath::Lerp(MaxAirControl, MinAirControl, Percent / static_cast<float>(MaxPercent));
	MovementComponent->AirControl = AirControl;

	// 넉백 적용 (서버에서만 직접 호출, 클라이언트에서는 RPC 호출)
	if (GetOwner()->HasAuthority())
	{
		// 직접 LaunchCharacter 함수 호출
		LaunchCharacter(KnockbackVector, true, true);
	}
	else
	{
		// 클라이언트에서는 RPC 호출
		Server_LaunchCharacter(KnockbackVector, true, true);
	}
}

// 캐릭터 런치 함수 (새로 추가)
void USmashCombatComponent::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (OwnerCharacter && IsValid(OwnerCharacter))
	{
		OwnerCharacter->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	}
}

// 서버 런치 함수 (새로 추가)
void USmashCombatComponent::Server_LaunchCharacter_Implementation(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

void USmashCombatComponent::SetPercent(int32 NewPercent)
{
	if (GetOwner()->HasAuthority())
	{
		int32 OldPercent = Percent;
		Percent = FMath::Clamp(NewPercent, 0, MaxPercent);

		// 값이 변경되었을 때만 이벤트 발생
		if (OldPercent != Percent)
		{
			OnPercentChanged.Broadcast(OldPercent, Percent);

			// SmashCharacterStats 동기화 (null 체크 추가)
			if (OwnerCharacter && OwnerCharacter->SmashCharacterStatsComponent)
			{
				OwnerCharacter->SmashCharacterStatsComponent->Percent = Percent;
			}
		}
	}
}

void USmashCombatComponent::AddPercent(int32 PercentToAdd)
{
	if (GetOwner()->HasAuthority())
	{
		int32 OldPercent = Percent;
		Percent = FMath::Clamp(Percent + PercentToAdd, 0, MaxPercent);

		// 값이 변경되었을 때만 이벤트 발생
		if (OldPercent != Percent)
		{
			OnPercentChanged.Broadcast(OldPercent, Percent);

			// SmashCharacterStats 동기화 (null 체크 추가)
			if (OwnerCharacter && OwnerCharacter->SmashCharacterStatsComponent)
			{
				OwnerCharacter->SmashCharacterStatsComponent->Percent = Percent;
			}
		}
	}
}

void USmashCombatComponent::OnRep_Percent()
{
	// 클라이언트에서 Percent가 복제되었을 때 UI 업데이트 등 처리
	if (OwnerCharacter && OwnerCharacter->SmashCharacterStatsComponent)
	{
		OwnerCharacter->SmashCharacterStatsComponent->Percent = Percent;

		// UI 업데이트가 필요하면 HUD에도 알림 (null 체크 추가)
		if (OwnerCharacter->UW_HUDCharacterInfo)
		{
			OwnerCharacter->UW_HUDCharacterInfo->PlayShake();
		}
	}
}

void USmashCombatComponent::CheckOutOfBounds()
{
	if (!OwnerCharacter || !IsValid(OwnerCharacter))
	{
		return;
	}

	// 맵 경계 정보 사용
	FVector ActorLocation = OwnerCharacter->GetActorLocation();

	// 맵 밖으로 나갔는지 체크
	bool bOutOfBounds = FMath::Abs(ActorLocation.X) > MapBounds.X ||
		FMath::Abs(ActorLocation.Y) > MapBounds.Y ||
		ActorLocation.Z < -MapBounds.Z || ActorLocation.Z > MapBounds.Z;

	if (bOutOfBounds)
	{
		// 장외 사망 처리
		OnDeath.Broadcast();

		// 캐릭터 관련 처리 (스톡 감소 등)
		if (OwnerCharacter->SmashCharacterStatsComponent)
		{
			OwnerCharacter->SmashCharacterStatsComponent->Stock--;
			OwnerCharacter->SmashCharacterStatsComponent->Falls++;

			// 게임 인스턴스에 알림 (null 체크 추가)
			if (OwnerCharacter->SmashGameInstance && OwnerCharacter->PlayerNo >= 0)
			{
				// OwnerCharacter->SmashGameInstance->PlayerDeath(OwnerCharacter->PlayerNo);
			}

			// 시체 생성 및 부활 로직 구현 (기획에 따른 추가)
			if (OwnerCharacter->SmashCharacterStatsComponent->Stock > 0)
			{
				// 5초 후 시체 생성 및 부활 로직 호출
				FTimerHandle RespawnTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [this]()
				{
					if (OwnerCharacter && IsValid(OwnerCharacter))
					{
						// OwnerCharacter->SpawnDeadBody();
					}
				}, 5.0f, false);
			}
		}
	}
}

void USmashCombatComponent::PlayHitEffect(bool bIsKnockback)
{
	if (!OwnerCharacter || !IsValid(OwnerCharacter))
	{
		return;
	}

	// 넉백일 경우 추가 이펙트
	if (bIsKnockback)
	{
		// 강한 타격 이펙트
		HitFlash = 1.0f;

		// SmashCharacter의 HitFlash 값도 업데이트 (호환성 유지)
		OwnerCharacter->HitFlash = 1.0f;
	}
	else
	{
		// 일반 타격 이펙트
		HitFlash = 0.5f;

		// SmashCharacter의 HitFlash 값도 업데이트 (호환성 유지)
		OwnerCharacter->HitFlash = 0.5f;
	}

	// 이펙트 지속 시간 후 리셋
	GetWorld()->GetTimerManager().ClearTimer(HitEffectTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HitEffectTimerHandle, [this]()
	{
		HitFlash = 0.0f;

		// SmashCharacter의 HitFlash 값도 업데이트 (호환성 유지)
		if (OwnerCharacter && IsValid(OwnerCharacter))
		{
			OwnerCharacter->HitFlash = 0.0f;
		}
	}, 0.25f, false);
}

void USmashCombatComponent::UpdateHitEffect()
{
	if (!OwnerCharacter || !IsValid(OwnerCharacter) || !OwnerCharacter->Material)
	{
		return;
	}

	// 피격 효과 파라미터 설정
	OwnerCharacter->Material->SetScalarParameterValue(FName("Hit"), HitFlash);

	// 캐릭터가 히트 상태일 때 색상 설정
	if (OwnerCharacter->SmashStateSystem &&OwnerCharacter->SmashStateSystem->GetCurrentState() == ESmashPlayerStates::Hit)
	{
		OwnerCharacter->Material->SetVectorParameterValue(FName("FlashColor"), FLinearColor(0.6f, 0.0f, 0.03f, 1.0f));
	}
}

void USmashCombatComponent::SetInvulnerable(bool bNewInvulnerable, float Duration)
{
	if (GetOwner()->HasAuthority())
	{
		bIsInvulnerable = bNewInvulnerable;

		// 무적 시간이 지정된 경우 타이머 설정
		if (bNewInvulnerable && Duration > 0.0f)
		{
			FTimerHandle InvulnerabilityTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(InvulnerabilityTimerHandle, [this]()
			{
				bIsInvulnerable = false;

				// 캐릭터의 HitState 업데이트
				if (OwnerCharacter && IsValid(OwnerCharacter))
				{
					OwnerCharacter->HitStates = ESmashHitState::Normal;
				}
			}, Duration, false);
		}

		// 캐릭터의 HitState 변경
		if (OwnerCharacter && IsValid(OwnerCharacter))
		{
			OwnerCharacter->HitStates = bNewInvulnerable ? ESmashHitState::Invincible : ESmashHitState::Normal;
		}
	}
}
