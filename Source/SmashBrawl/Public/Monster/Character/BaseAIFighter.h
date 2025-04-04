#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAIFighter.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseAIFighter : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseAIFighter();

    virtual void BeginPlay() override;

    /** 어빌리티 입력 파라미터 설정 */
    void SetAbility(float InLeftRight, float InUpDown, int32 InParam, int32 InAbilityType, int32 InDirection);

    // --- 상태 판단 함수들 ---
    /** 지정 거리 이내에 있는 유효한 타겟 반환 */
    UFUNCTION(BlueprintCallable)
    AActor* GetValidTargetByDistance(AActor* TargetActor, AActor* ParentActor, float DistanceThreshold = 400.0f);

    /** 타겟이 지정한 Y/Z 범위 안에 있는지 확인 */
    UFUNCTION(BlueprintCallable)
    bool IsTargetInRange(AActor* TargetActor, AActor* ParentActor,
        float YMin, float YMax,
        float ZMin, float ZMax);

    /** 캐릭터가 행동 가능한 상태인지 여부 */
    UFUNCTION(BlueprintCallable, Category = "AI|State")
    bool IsActive() const;

    /** 회피 상태인지 여부 */
    UFUNCTION(BlueprintCallable, Category = "AI|State")
    bool IsInMoveAwayMode() const;

    /** 리커버리 시작 (리스폰 위치로 이동) */
    UFUNCTION(BlueprintCallable)
    void StartRecovery();

    /** 리커버리 상태 리셋 (딜레이 기반) */
    UFUNCTION()
    void ResetRecovery();

    /** 리커버리 종료 → 정상 상태 복귀 */
    UFUNCTION()
    void FinishRecovery();

    /** CPU AI 여부 */
    UFUNCTION(BlueprintCallable, Category = "AI|State")
    bool IsCPU() const;

    /** 스테이지 밖으로 떨어졌는지 여부 */
    UFUNCTION(BlueprintCallable, Category = "AI|State")
    bool IsOffStage() const;

protected:
    void SetStateFromParam(int32 Param);

    /** 리커버리 딜레이 타이머 핸들 */
    FTimerHandle RecoveryHandle;

    // --- 상태 변수들 ---

    /** 리커버리 중인지 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    bool bRecoverMode = false;

    /** 리커버리 리셋 타이머 진행 중인지 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    bool bResetRecoveryInProgress = false;

    /** 현재 캐릭터가 행동 가능한 상태인지 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool bActive = true;

    /** 회피 상태 모드 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
    bool bMoveAwayMode = false;

    /** CPU(AI)인지 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
    bool bIsCPU = true;

    /** 땅 위에 있는 상태인지 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
    bool bOverGround = true;

    /** 리스폰 위치 저장용 */
    FVector RespawnLocation;

    /** 리스폰 위치 방향으로 이동 */
    void MoveTowardRespawn();

public:
    // --- 어빌리티용 입력 값들 (네트워크 동기화 포함) ---

    /** 좌우 입력 */
    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    float LeftRight;

    /** 상하 입력 */
    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    float UpDown;

    /** 어빌리티 종류 */
    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    int32 AbilityType;

    /** 방향 입력 */
    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    int32 Direction;
};
