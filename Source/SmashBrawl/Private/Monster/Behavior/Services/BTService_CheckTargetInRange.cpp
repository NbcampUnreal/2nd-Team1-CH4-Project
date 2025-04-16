#include "Monster/Behavior/Services/BTService_CheckTargetInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Character/BaseAIFighter.h"
#include "DrawDebugHelpers.h"

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

    const float XMin = -200.f;
    const float XMax = 200.f;
    const float ZMin = -50.f;
    const float ZMax = 50.f;

    bool bInRange = AIPawn->IsTargetInRange(Target, Parent, XMin, XMax, ZMin, ZMax);

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bInRange);

}
