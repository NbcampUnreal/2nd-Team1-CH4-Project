#pragma once

#include "CoreMinimal.h"
#include "SmashAction.h"
#include "PlayMontageAction.generated.h"

/**
 * 애니메이션 몽타주를 재생하는 액션 클래스
 * 몽타주가 완료되면 지정된 다음 상태로 자동 전환할 수 있음
 */
UCLASS()
class SMASHBRAWL_API UPlayMontageAction : public USmashAction
{
	GENERATED_BODY()

public:
	UPlayMontageAction();

	// USmashAction 인터페이스 구현
	virtual void InitAction_Implementation(UBaseCharacterState* InOwnerState) override;
	virtual bool Execute_Implementation() override;
	virtual bool CanExecute_Implementation() override;

	// 몽타주 재생 함수
	UFUNCTION(BlueprintCallable, Category = "Action|Montage")
	bool PlayMontage();

	// 현재 재생 중인 몽타주 중단
	UFUNCTION(BlueprintCallable, Category = "Action|Montage")
	bool StopMontage(float InBlendOutTime = 0.25f);

	// 몽타주 종료 콜백 - UFUNCTION() 매크로 추가
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 몽타주 재생 진행 상황 반환 (0.0 ~ 1.0)
	UFUNCTION(BlueprintPure, Category = "Action|Montage")
	float GetMontagePlayProgress() const;

	// 리소스 정리 함수
	UFUNCTION(BlueprintCallable, Category = "Action|Montage")
	void CleanupDelegates();

public:
	// 몽타주 애니메이션이 끝났을 때 다음 상태로 전환할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage")
	bool bAnimEndNext;

	// 다음으로 전환할 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage", meta = (EditCondition = "bAnimEndNext"))
	ESmashPlayerStates NextState;

	// 재생할 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage")
	UAnimMontage* Montage;

	// 몽타주 재생 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float PlayRate;

	// 시작할 섹션 이름 (비어 있으면 처음부터 재생)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage")
	FName StartSectionName;

	// 몽타주가 이미 재생 중일 때 다시 시작할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage")
	bool bRestartIfPlaying;

	// 몽타주 블렌딩 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BlendOutTime;

	// 몽타주가 인터럽트되었을 때도 다음 상태로 전환할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action|Montage", meta = (EditCondition = "bAnimEndNext"))
	bool bChangeStateOnInterrupted;

protected:
	// 몽타주 재생 중인지 여부
	bool bIsPlaying;

	// 몽타주 재생 ID (중지할 때 사용)
	FDelegateHandle MontageEndedDelegateHandle;
};