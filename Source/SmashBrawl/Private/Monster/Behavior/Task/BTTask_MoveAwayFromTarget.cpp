#include "Monster/Behavior/Task/BTTask_MoveAwayFromTarget.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveAwayFromTarget::UBTTask_MoveAwayFromTarget()
{
    NodeName = TEXT("Move Away From Target");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveAwayFromTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!AIPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp) return EBTNodeResult::Failed;

    AActor* Target = Cast<AActor>(BBComp->GetValueAsObject(GetSelectedBlackboardKey()));
    if (!Target) return EBTNodeResult::Failed;

    FVector MyLocation = AIPawn->GetActorLocation();
    FVector TargetLocation = Target->GetActorLocation();
    FVector Direction = (MyLocation - TargetLocation).GetSafeNormal2D();
    float Distance = FVector::Dist(MyLocation, TargetLocation);

    if (Distance > StopDistance)
    {
        return EBTNodeResult::Succeeded;
    }

    AIPawn->AddMovementInput(Direction, MoveSpeed);
    return EBTNodeResult::InProgress;
}

void UBTTask_MoveAwayFromTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    AActor* Target = Cast<AActor>(BBComp->GetValueAsObject(GetSelectedBlackboardKey()));

    if (!AIController || !AIPawn || !Target)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector MyLocation = AIPawn->GetActorLocation();
    FVector TargetLocation = Target->GetActorLocation();
    float Distance = FVector::Dist(MyLocation, TargetLocation);

    if (Distance > StopDistance)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        FVector Direction = (MyLocation - TargetLocation).GetSafeNormal2D();
        AIPawn->AddMovementInput(Direction, MoveSpeed);
    }
}