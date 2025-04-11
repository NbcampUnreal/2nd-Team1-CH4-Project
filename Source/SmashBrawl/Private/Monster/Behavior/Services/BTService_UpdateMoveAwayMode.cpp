#include "Monster/Behavior/Services/BTService_UpdateMoveAwayMode.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateMoveAwayMode::UBTService_UpdateMoveAwayMode()
{
    NodeName = TEXT("Update MoveAwayMode");
}

void UBTService_UpdateMoveAwayMode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    ABaseAIFighter* AIPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    if (!AIController || !AIPawn || !BBComp) return;

    const bool bShouldMoveAway = AIPawn->IsInMoveAwayMode();
    BBComp->SetValueAsBool(GetSelectedBlackboardKey(), bShouldMoveAway);
}