

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

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};