#include "AI/Controller/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ABaseAIController::ABaseAIController()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();
    // 초기화 등 필요한 작업 가능
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // TODO: BehaviorTreeAsset은 블루프린트나 외부에서 설정
    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
        InitializeBlackboard();
    }
}

void ABaseAIController::InitializeBlackboard()
{
    // Blackboard Key 초기화 등 설정
}
