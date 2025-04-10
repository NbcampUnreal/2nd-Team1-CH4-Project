

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTTask_MoveToTarget();
    void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
    bool bIsMoving = true;
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
    UPROPERTY(EditAnywhere, Category = "AI")
    float MoveSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float StopDistance = 100.f;
};
