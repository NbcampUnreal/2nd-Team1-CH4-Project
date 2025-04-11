

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateMoveAwayMode.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBTService_UpdateMoveAwayMode : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTService_UpdateMoveAwayMode();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
