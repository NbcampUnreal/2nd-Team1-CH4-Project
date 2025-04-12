

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveAwayFromTarget.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBTTask_MoveAwayFromTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTTask_MoveAwayFromTarget();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, Category = "AI")
    float MoveSpeed = 1.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float EscapeDistance = 300.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float StopDistance = 600.f;
};
