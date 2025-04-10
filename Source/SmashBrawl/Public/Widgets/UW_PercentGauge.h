#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PercentGauge.generated.h"

class USmashCharacterStats;
class UTextBlock;
class UProgressBar;
class ASmashCharacter;

/**
 * 캐릭터의 피격 퍼센트를 보여주는 게이지 위젯
 */
UCLASS()
class SMASHBRAWL_API UUW_PercentGauge : public UUserWidget
{
    GENERATED_BODY()

public:
    UUW_PercentGauge(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 캐릭터 설정
    UFUNCTION(BlueprintCallable, Category = "Percent Gauge")
    void SetOwnerCharacter(ASmashCharacter* NewOwnerCharacter);

    // 퍼센트 수동 업데이트
    UFUNCTION(BlueprintCallable, Category = "Percent Gauge")
    void UpdatePercentGauge();

    // 퍼센트 변경 이벤트 핸들러 - UFUNCTION 마크 추가
    UFUNCTION()
    void OnPercentChanged(int32 OldPercent, int32 NewPercent);

protected:
    // UI 컴포넌트
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UProgressBar> PercentProgressBar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> PercentText;

    // 소유 캐릭터 참조
    UPROPERTY(BlueprintReadWrite, Category = "Percent Gauge")
    TObjectPtr<ASmashCharacter> OwnerCharacter;

    // 캐릭터 스탯 컴포넌트 참조
    UPROPERTY(BlueprintReadWrite, Category = "Percent Gauge")
    TObjectPtr<USmashCharacterStats> CharacterStats;

    // 프로그래스 바 색상 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Appearance")
    FLinearColor LowPercentColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Appearance")
    FLinearColor MediumPercentColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Appearance")
    FLinearColor HighPercentColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Appearance")
    FLinearColor DangerPercentColor;

    // 색상 변경 임계값 (퍼센트 단위, 0-100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Thresholds", meta = (ClampMin = "0", ClampMax = "100"))
    float MediumPercentThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Thresholds", meta = (ClampMin = "0", ClampMax = "100"))
    float HighPercentThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Thresholds", meta = (ClampMin = "0", ClampMax = "100"))
    float DangerPercentThreshold;

    // 애니메이션 처리 변수
    UPROPERTY(BlueprintReadWrite, Category = "Percent Gauge|Animation")
    float CurrentAnimatedPercent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Percent Gauge|Animation")
    float AnimationSpeed;

    // 델리게이트 연결 상태 추적
    UPROPERTY()
    bool bIsListeningToPercentChanged = false;
};