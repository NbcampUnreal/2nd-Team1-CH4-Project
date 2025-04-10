

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseAbility.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBTTask_UseAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTTask_UseAbility();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector Key_LeftRight;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector Key_UpDown;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector Key_Param;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector Key_Type;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector Key_Direction;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};