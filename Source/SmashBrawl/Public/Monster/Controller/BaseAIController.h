#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class ASmashCharacter;
class ABaseCharacter;
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
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBlackboardData* BlackboardAsset;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Player")
    TArray<TObjectPtr<ASmashCharacter>> PlayerArray;

    UPROPERTY()
    class ABaseAIFighter* ControlledFighter;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBlackboardComponent* BlackboardComponent;
};
