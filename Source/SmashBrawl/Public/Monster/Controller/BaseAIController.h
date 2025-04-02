#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABaseAIController();

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

protected:
    void InitializeBlackboard();

private:
    UPROPERTY()
    class UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY()
    class UBlackboardComponent* BlackboardComponent;
};
