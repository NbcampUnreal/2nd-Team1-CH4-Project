#include "Monster/Behavior/Task/BTTask_FaceOnAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"

UBTTask_FaceOnAttack::UBTTask_FaceOnAttack()
{
    NodeName = TEXT("Face On Attack");
}

EBTNodeResult::Type UBTTask_FaceOnAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!AIPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!TargetActor) return EBTNodeResult::Failed;

    FVector ToTarget = TargetActor->GetActorLocation() - AIPawn->GetActorLocation();
    float Direction = ToTarget.X;

    // 반대 방향이면 회전
    if ((Direction > 0 && !AIPawn->IsFacingRight()) || (Direction < 0 && AIPawn->IsFacingRight()))
    {
        AIPawn->FlipDirection();
    }

    return EBTNodeResult::Succeeded;
}
