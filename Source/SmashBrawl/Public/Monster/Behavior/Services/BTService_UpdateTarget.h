

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateTarget.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBTService_UpdateTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTService_UpdateTarget();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
