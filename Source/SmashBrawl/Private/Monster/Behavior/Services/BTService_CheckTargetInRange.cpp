#include "Monster/Behavior/Services/BTService_CheckTargetInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Character/BaseAIFighter.h"

UBTService_CheckTargetInRange::UBTService_CheckTargetInRange()
{
    NodeName = TEXT("Check Target In Range");
}

void UBTService_CheckTargetInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!AIPawn) return;

    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
    AActor* Parent = AIPawn;

    if (!Target || !Parent) return;

    const float YMin = -110.f;
    const float YMax = 110.f;
    const float ZMin = -10.f;
    const float ZMax = 10.f;

    bool bInRange = AIPawn->IsTargetInRange(Target, Parent, YMin, YMax, ZMin, ZMax);

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bInRange);
}

