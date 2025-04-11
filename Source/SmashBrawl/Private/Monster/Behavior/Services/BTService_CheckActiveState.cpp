#include "Monster/Behavior/Services/BTService_CheckActiveState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Character/BaseAIFighter.h"

UBTService_CheckActiveState::UBTService_CheckActiveState()
{
    NodeName = TEXT("Check bActive State");
    Interval = 0.5f;
    RandomDeviation = 0.0f;
}

void UBTService_CheckActiveState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller) return;

    ABaseAIFighter* Fighter = Cast<ABaseAIFighter>(Controller->GetPawn());
    if (!Fighter) return;

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return;

    Blackboard->SetValueAsBool(FName("IsActive"), Fighter->IsActive());
}
