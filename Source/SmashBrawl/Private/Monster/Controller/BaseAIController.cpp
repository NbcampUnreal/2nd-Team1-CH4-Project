#include "Monster/Controller/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"
#include "SmashBrawl/Public/Charactor/BaseCharacter.h"
#include "Monster/Character/BaseAIFighter.h"

ABaseAIController::ABaseAIController()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();

    if (UseBlackboard(BlackboardAsset, BlackboardComponent))
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (ABaseCharacter* Char = Cast<ABaseCharacter>(Actor))
        {
            PlayerArray.Add(Char);
        }
    }
    if (PlayerArray.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, PlayerArray.Num() - 1);
        ABaseCharacter* RandomCharacter = PlayerArray[RandomIndex];

        BlackboardComponent->SetValueAsObject(FName("Target"), RandomCharacter);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerArray에 Player 없음"));
    }

void ABaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ControlledFighter = Cast<ABaseAIFighter>(InPawn);

    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
        InitializeBlackboard();
    }

    // bActive 초기 블랙보드 세팅
    if (ControlledFighter && BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(FName("IsActive"), ControlledFighter->IsActive());
    }
}

void ABaseAIController::InitializeBlackboard()
{
}
