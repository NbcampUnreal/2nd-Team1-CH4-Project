

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FaceOnAttack.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBTTask_FaceOnAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_FaceOnAttack();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetKey;
};
