#include "Monster/Behavior/Services/BTService_UpdateTarget.h"
#include "Monster/Character/BaseAIFighter.h"
#include "Monster/Controller/BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTarget::UBTService_UpdateTarget()
{
    NodeName = TEXT("Update Target (Closest)");
}

void UBTService_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    ABaseAIFighter* SelfPawn = Cast<ABaseAIFighter>(AIController->GetPawn());
    if (!SelfPawn) return;

    TArray<AActor*> Fighters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Fighters); // 타겟설정

    AActor* Closest = nullptr;
    float MinDist = FLT_MAX;

    for (AActor* Actor : Fighters)
    {
        if (Actor == SelfPawn) continue;

        if (!Actor->Tags.Contains(FName("PlayerCharacter"))) continue;

        ACharacter* Character = Cast<ACharacter>(Actor);
        if (!Character || !Character->GetController()->IsPlayerController()) continue;

        float Dist = FVector::Dist(Actor->GetActorLocation(), SelfPawn->GetActorLocation());
        if (Dist < MinDist)
        {
            MinDist = Dist;
            Closest = Actor;
        }
    }

    if (Closest)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Closest);
    }
}