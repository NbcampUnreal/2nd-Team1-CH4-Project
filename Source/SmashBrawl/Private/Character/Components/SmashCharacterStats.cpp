#include "Character/Components/SmashCharacterStats.h"

#include "Character/SmashCharacter.h"
#include "Core/SmashGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/HUD/InGame/UW_HUD_CharacterInfo.h"

// Sets default values for this component's properties
USmashCharacterStats::USmashCharacterStats()
{
	// 필요한 경우에만 Tick을 활성화
	PrimaryComponentTick.bCanEverTick = false;


	// 기본값 초기화
	Percent = 0;
	MaxPercent = 1000;
	Stock = 10; // 기본 스톡 수
	DefaultGravityScale = 1.0f;
	Weight = 100.0f;
	SuperIndex = 0;
	Elims = 0;
	DamageDone = 0;
	DamageTaken = 0;
	SD = 0;
	Falls = 0;
	bIsInvulnerable = false;
	
	// 자연 회복 관련 변수 초기화
	bEnableNaturalRecovery = true;
	NaturalRecoveryRate = 1;   // 초당 1씩 감소
	NaturalRecoveryInterval = 1.0f; // 1초마다 업데이트

	SetIsReplicatedByDefault(true);
}

void USmashCharacterStats::BeginPlay()
{
	Super::BeginPlay();
	
	Stock = 10; // 기본 스톡 수

	// Parent 변수 초기화 (중복 제거)
	Parent = Cast<ASmashCharacter>(GetOwner());
	if (!Parent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SmashCharacterStats: Owner is not a SmashCharacter!"));
		return;
	}

	// 게임 인스턴스에서 스톡 설정 가져오기
	USmashGameInstance* SmashGameInstance = Cast<USmashGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (SmashGameInstance && SmashGameInstance->SubMode == ESmashInGameMode::Stock)
	{
		Stock = SmashGameInstance->StockCount;
	}
	
	// 자연 회복 타이머 초기 설정
	UpdateNaturalRecoveryTimer();
}

void USmashCharacterStats::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	// 모든 타이머 정리
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(InvulnerabilityTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(NaturalRecoveryTimerHandle);
	}
}

void USmashCharacterStats::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 속성 등록
	DOREPLIFETIME(USmashCharacterStats, Percent);
	DOREPLIFETIME(USmashCharacterStats, Stock);
	DOREPLIFETIME(USmashCharacterStats, bIsInvulnerable);
}

void USmashCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Tick이 비활성화되어 이 함수는 호출되지 않지만, 오버라이드는 유지
}

void USmashCharacterStats::ProcessNaturalRecovery()
{
	// 서버에서만 실행 (권한 체크)
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	// 자연 회복 비활성화 상태인 경우 처리하지 않음
	if (!bEnableNaturalRecovery)
	{
		return;
	}
	
	// 현재 Percent가 0 이하면 처리하지 않음
	if (Percent <= 0)
	{
		return;
	}
	
	// Percent 감소
	int32 OldPercent = Percent;
	Percent = FMath::Max(0, Percent - NaturalRecoveryRate);
	
	// 값이 변경되었을 때만 이벤트 발생
	if (OldPercent != Percent)
	{
		OnPercentChanged.Broadcast(OldPercent, Percent);
		
		// Percent가 0이 되면 타이머 중지
		if (Percent == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(NaturalRecoveryTimerHandle);
		}
	}
}

void USmashCharacterStats::UpdateNaturalRecoveryTimer()
{
	// 서버에서만 실행 (권한 체크)
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
	
	// 타이머 관리
	if (bEnableNaturalRecovery && Percent > 0)
	{
		// 기존 타이머가 활성화되어 있지 않다면 타이머 시작
		if (!GetWorld()->GetTimerManager().IsTimerActive(NaturalRecoveryTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				NaturalRecoveryTimerHandle,
				this,
				&USmashCharacterStats::ProcessNaturalRecovery,
				NaturalRecoveryInterval,
				true  // 반복 실행
			);
		}
	}
	else
	{
		// 타이머가 활성화되어 있으면 중지
		if (GetWorld()->GetTimerManager().IsTimerActive(NaturalRecoveryTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(NaturalRecoveryTimerHandle);
		}
	}
}

void USmashCharacterStats::Screenshake(float RumbleInten, float RumbleDuration)
{
	if (!Parent || !ShakeClass)
	{
		return;
	}

	UGameplayStatics::PlayWorldCameraShake(this, ShakeClass, Parent->GetActorLocation(), RumbleInten, RumbleDuration);
}

void USmashCharacterStats::OnRep_Percent()
{
	// UI 업데이트 등 추가 작업 (null 체크 추가)
	OnPercentChanged.Broadcast(0, Percent);
	
	if (Parent && Parent->UW_HUDCharacterInfo)
	{
		Parent->UW_HUDCharacterInfo->PlayShake();
	}
}

void USmashCharacterStats::OnRep_IsInvulnerable()
{
	// 무적 상태 변경 시 시각적 효과 업데이트
	if (Parent)
	{
		Parent->HitStates = bIsInvulnerable ? ESmashHitState::Invincible : ESmashHitState::Normal;
	}
}

void USmashCharacterStats::SetPercent(int32 NewPercent)
{
	if (GetOwner()->HasAuthority())
	{
		int32 OldPercent = Percent;
		Percent = FMath::Clamp(NewPercent, 0, MaxPercent);

		// 값이 변경되었을 때만 이벤트 발생
		if (OldPercent != Percent)
		{
			OnPercentChanged.Broadcast(OldPercent, Percent);
			
			// Percent가 변경되면 자연 회복 타이머 업데이트
			UpdateNaturalRecoveryTimer();
		}
	}
}

void USmashCharacterStats::AddPercent(int32 PercentToAdd)
{
	if (GetOwner()->HasAuthority())
	{
		int32 OldPercent = Percent;
		Percent = FMath::Clamp(Percent + PercentToAdd, 0, MaxPercent);

		// 값이 변경되었을 때만 이벤트 발생
		if (OldPercent != Percent)
		{
			OnPercentChanged.Broadcast(OldPercent, Percent);
			DamageTaken += PercentToAdd;
			SuperIndex += PercentToAdd * 0.5f;
			
			// Percent가 변경되면 자연 회복 타이머 업데이트
			UpdateNaturalRecoveryTimer();
		}
	}
}

void USmashCharacterStats::SetSuper(int32 NewSuper)
{
	if (GetOwner()->HasAuthority())
	{
		SuperIndex = NewSuper;
	}
}

void USmashCharacterStats::SetInvulnerable(bool bNewInvulnerable, float Duration)
{
	if (GetOwner()->HasAuthority())
	{
		bIsInvulnerable = bNewInvulnerable;

		// 기존 타이머가 있다면 정리
		if (GetWorld()->GetTimerManager().IsTimerActive(InvulnerabilityTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(InvulnerabilityTimerHandle);
		}

		// 무적 시간이 지정된 경우 타이머 설정
		if (bNewInvulnerable && Duration > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(InvulnerabilityTimerHandle, [this]()
			{
				bIsInvulnerable = false;

				// 캐릭터의 HitState 업데이트
				if (Parent && IsValid(Parent))
				{
					Parent->HitStates = ESmashHitState::Normal;
				}
			}, Duration, false);
		}

		// 캐릭터의 HitState 변경
		if (Parent && IsValid(Parent))
		{
			Parent->HitStates = bNewInvulnerable ? ESmashHitState::Invincible : ESmashHitState::Normal;
		}
	}
}