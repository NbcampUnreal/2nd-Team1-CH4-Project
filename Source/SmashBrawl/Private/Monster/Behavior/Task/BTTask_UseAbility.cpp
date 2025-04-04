#include "Monster/Behavior/Task/BTTask_UseAbility.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"

UBTTask_UseAbility::UBTTask_UseAbility()
{
    NodeName = TEXT("Use Ability");
}

EBTNodeResult::Type UBTTask_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ABaseAIFighter* AIFighter = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!AIFighter) return EBTNodeResult::Failed;

    // 임시 테스트용 파라미터
    float LeftRight = 1.0f;
    float UpDown = 0.0f;
    int32 Param = 0;
    int32 AbilityType = 0;
    int32 Direction = 1;

    AIFighter->SetAbility(LeftRight, UpDown, Param, AbilityType, Direction);

    return EBTNodeResult::Succeeded;
}