

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
