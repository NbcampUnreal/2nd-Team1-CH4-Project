#include "Monster/Behavior/Task/BTTask_UseAbility.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_UseAbility::UBTTask_UseAbility()
{
    NodeName = TEXT("Use Ability");
}

EBTNodeResult::Type UBTTask_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!AIPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp) return EBTNodeResult::Failed;

    // Blackboard에서 값 가져오기
    float LeftRight = BBComp->GetValueAsFloat(Key_LeftRight.SelectedKeyName);
    float UpDown = BBComp->GetValueAsFloat(Key_UpDown.SelectedKeyName);
    int32 Param = BBComp->GetValueAsInt(Key_Param.SelectedKeyName);
    int32 Type = BBComp->GetValueAsInt(Key_Type.SelectedKeyName);
    int32 Direction = BBComp->GetValueAsInt(Key_Direction.SelectedKeyName);

    // 능력 실행
    AIPawn->SetAbility(LeftRight, UpDown, Param, Type, Direction);

    return EBTNodeResult::Succeeded;
}
