#include "Monster/Behavior/Task/BTTask_Attack.h"
#include "Monster/Controller/BaseAIController.h"
#include "Monster/Character/BaseAIFighter.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    ABaseAIFighter* AIFighter = Cast<ABaseAIFighter>(Controller->GetPawn());

    if (AIFighter)
    {
        AIFighter->PlayRandomAttackMontage();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
