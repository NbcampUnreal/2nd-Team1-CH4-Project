#include "Widgets/UW_PercentGauge.h"
#include "Character/SmashCharacter.h"
#include "Character/Components/SmashCharacterStats.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UUW_PercentGauge::UUW_PercentGauge(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 기본값 초기화
	LowPercentColor = FLinearColor(0.0f, 0.7f, 0.0f, 1.0f); // 녹색
	MediumPercentColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // 노란색
	HighPercentColor = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // 주황색
	DangerPercentColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // 빨간색

	// 퍼센트 단위로 임계값 설정 (0-100%)
	MediumPercentThreshold = 30.0f;
	HighPercentThreshold = 50.0f;
	DangerPercentThreshold = 80.0f;

	CurrentAnimatedPercent = 0.0f;
	AnimationSpeed = 5.0f;
}

void UUW_PercentGauge::NativeConstruct()
{
	Super::NativeConstruct();

	// UI 컴포넌트 유효성 체크
	if (!PercentProgressBar || !PercentText)
	{
		UE_LOG(LogTemp, Warning, TEXT("UW_PercentGauge: UI 요소가 바인딩되지 않았습니다."));
	}

	// 기본 UI 설정
	if (PercentProgressBar)
	{
		PercentProgressBar->SetFillColorAndOpacity(LowPercentColor);
	}

	if (PercentText)
	{
		PercentText->SetText(FText::FromString(TEXT("0%")));
	}
}

void UUW_PercentGauge::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 캐릭터 스탯 참조가 없으면 업데이트 건너뜀
	if (!CharacterStats)
	{
		return;
	}

	// 애니메이션 처리 - 실제 값으로 부드럽게 이동
	float TargetPercent = CharacterStats->GetPercent();
	if (!FMath::IsNearlyEqual(CurrentAnimatedPercent, TargetPercent, 0.1f))
	{
		// 애니메이션 속도로 현재 값 업데이트
		CurrentAnimatedPercent = FMath::FInterpTo(CurrentAnimatedPercent, TargetPercent, InDeltaTime, AnimationSpeed);

		// UI 업데이트
		float NormalizedValue = FMath::Clamp(CurrentAnimatedPercent / (CharacterStats->MaxPercent), 0.0f, 1.0f);

		if (PercentProgressBar)
		{
			PercentProgressBar->SetPercent(NormalizedValue);
		}

		if (PercentText)
		{
			// 텍스트를 실제 퍼센트 값으로 표시하고 % 기호 추가
			int32 CurrentPercentInt = FMath::RoundToInt(CurrentAnimatedPercent);
			PercentText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), CurrentPercentInt)));
		}
	}
}

void UUW_PercentGauge::SetOwnerCharacter(ASmashCharacter* NewOwnerCharacter)
{
	// 기존 델리게이트 연결 해제
	if (CharacterStats && bIsListeningToPercentChanged)
	{
		CharacterStats->OnPercentChanged.RemoveDynamic(this, &UUW_PercentGauge::OnPercentChanged);
		bIsListeningToPercentChanged = false;
	}

	// 새 캐릭터 설정
	OwnerCharacter = NewOwnerCharacter;

	if (OwnerCharacter)
	{
		CharacterStats = OwnerCharacter->SmashCharacterStatsComponent;

		// 새 델리게이트 연결
		if (CharacterStats)
		{
			CharacterStats->OnPercentChanged.AddDynamic(this, &UUW_PercentGauge::OnPercentChanged);
			bIsListeningToPercentChanged = true;

			// 초기 UI 업데이트
			UpdatePercentGauge();
		}
	}
	else
	{
		CharacterStats = nullptr;
	}
}

void UUW_PercentGauge::UpdatePercentGauge()
{
	if (!CharacterStats || !PercentProgressBar || !PercentText)
	{
		return;
	}

	// 현재 퍼센트 값 가져오기
	int32 CurrentPercent = FMath::RoundToInt(CharacterStats->GetPercent());
	CurrentAnimatedPercent = CurrentPercent; // 즉시 업데이트

	// 프로그래스 바 값 설정
	float NormalizedValue = FMath::Clamp(static_cast<float>(CurrentPercent) / static_cast<float>(CharacterStats->MaxPercent), 0.0f, 1.0f);
	PercentProgressBar->SetPercent(NormalizedValue);

	// 텍스트 업데이트 - 실제 퍼센트 값으로 표시하고 % 기호 추가
	PercentText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), CurrentPercent)));

	// 색상 설정 - 절대 퍼센트 값을 기준으로 임계값 사용
	FLinearColor BarColor;

	// 캐릭터의 최대 퍼센트를 기준으로 현재 퍼센트의 상대적 비율 계산 (0-100%)
	float RelativePercent = (CurrentPercent * 100.0f) / CharacterStats->MaxPercent;

	if (RelativePercent >= DangerPercentThreshold)
	{
		BarColor = DangerPercentColor;
	}
	else if (RelativePercent >= HighPercentThreshold)
	{
		BarColor = HighPercentColor;
	}
	else if (RelativePercent >= MediumPercentThreshold)
	{
		BarColor = MediumPercentColor;
	}
	else
	{
		BarColor = LowPercentColor;
	}

	PercentProgressBar->SetFillColorAndOpacity(BarColor);
}

void UUW_PercentGauge::OnPercentChanged(int32 OldPercent, int32 NewPercent)
{
	// 퍼센트 변경 시 색상 처리만 즉시 업데이트
	// 애니메이션은 Tick에서 처리됨

	if (!PercentProgressBar || !CharacterStats)
	{
		return;
	}

	// 캐릭터의 최대 퍼센트를 기준으로 현재 퍼센트의 상대적 비율 계산 (0-100%)
	float RelativePercent = (NewPercent * 100.0f) / CharacterStats->MaxPercent;

	FLinearColor BarColor;

	if (RelativePercent >= DangerPercentThreshold)
	{
		BarColor = DangerPercentColor;
	}
	else if (RelativePercent >= HighPercentThreshold)
	{
		BarColor = HighPercentColor;
	}
	else if (RelativePercent >= MediumPercentThreshold)
	{
		BarColor = MediumPercentColor;
	}
	else
	{
		BarColor = LowPercentColor;
	}

	PercentProgressBar->SetFillColorAndOpacity(BarColor);
}
