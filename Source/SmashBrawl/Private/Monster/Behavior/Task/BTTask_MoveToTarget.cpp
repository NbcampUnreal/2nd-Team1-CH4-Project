#include "Monster/Behavior/Task/BTTask_MoveToTarget.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
    NodeName = TEXT("Move To Target");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!AIPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp) {
        UE_LOG(LogTemp, Warning, TEXT("bbcomp"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Warning, TEXT("선택된 키 이름 : % s"), *GetSelectedBlackboardKey().ToString());
    AActor* Target = Cast<AActor>(BBComp->GetValueAsObject(GetSelectedBlackboardKey()));
    if (!Target)
    {
        UE_LOG(LogTemp, Warning, TEXT("Target"));
        return EBTNodeResult::Failed;
    }

    FVector MyLocation = AIPawn->GetActorLocation();
    FVector TargetLocation = Target->GetActorLocation();
    FVector Direction = (TargetLocation - MyLocation).GetSafeNormal2D();
    float Distance = FVector::Dist(MyLocation, TargetLocation);

    UE_LOG(LogTemp, Warning, TEXT("Distance to Target: %f / StopDistance: %f"), Distance, StopDistance);

    if (Distance < StopDistance)
    {
        UE_LOG(LogTemp, Warning, TEXT("너무 가까워서 이동하지 않음"));
        return EBTNodeResult::Succeeded;
    }

    UE_LOG(LogTemp, Warning, TEXT("AddMovementInput 호출 전 (ExecuteTask)"));
    AIPawn->AddMovementInput(Direction, MoveSpeed);
    return EBTNodeResult::InProgress;
}

void UBTTask_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UE_LOG(LogTemp, Warning, TEXT("ticktask"));
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
    if (Distance < StopDistance)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        FVector Direction = (TargetLocation - MyLocation).GetSafeNormal2D();
        UE_LOG(LogTemp, Warning, TEXT("📍 이동 중: Dir=%s"), *Direction.ToString());
        AIPawn->AddMovementInput(Direction, MoveSpeed);
    }
}
