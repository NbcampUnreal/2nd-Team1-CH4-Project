

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

    /** 어빌리티 관련 파라미터 설정 */
    void SetAbility(float InLeftRight, float InUpDown, int32 InParam, int32 InAbilityType, int32 InDirection);

    // Target과 Parent 간 거리 체크
    UFUNCTION(BlueprintCallable)
    AActor* GetValidTargetByDistance(AActor* TargetActor, AActor* ParentActor, float DistanceThreshold = 400.0f);

    // Target과 Parent 위치를 비교
    UFUNCTION(BlueprintCallable)
    bool IsTargetInRange(AActor* TargetActor, AActor* ParentActor,
        float YMin, float YMax,
        float ZMin, float ZMax);


protected:
    void SetStateFromParam(int32 Param);

public:
    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    float LeftRight;

    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    float UpDown;

    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    int32 AbilityType;

    UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
    int32 Direction;
};
