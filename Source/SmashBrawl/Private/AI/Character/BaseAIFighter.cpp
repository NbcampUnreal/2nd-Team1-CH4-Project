#include "AI/Character/BaseAIFighter.h"
#include "Net/UnrealNetwork.h"

ABaseAIFighter::ABaseAIFighter()
{
    bReplicates = true;
}

void ABaseAIFighter::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseAIFighter::SetAbility(float InLeftRight, float InUpDown, int32 InParam, int32 InAbilityType, int32 InDirection)
{
    LeftRight = InLeftRight;
    UpDown = InUpDown;
    AbilityType = InAbilityType;
    Direction = InDirection;

    SetStateFromParam(InParam);
}

AActor* ABaseAIFighter::GetValidTargetByDistance(AActor* TargetActor, AActor* ParentActor, float DistanceThreshold)
{
    if (!TargetActor || !ParentActor)
    {
        return nullptr;
    }

    const float Distance = TargetActor->GetDistanceTo(ParentActor);
    return (Distance >= DistanceThreshold) ? TargetActor : this;
}

bool ABaseAIFighter::IsTargetInRange(AActor* TargetActor, AActor* ParentActor, float YMin, float YMax, float ZMin, float ZMax)
{
    if (!TargetActor || !ParentActor)
    {
        return false;
    }

    const FVector TargetLocation = TargetActor->GetActorLocation();
    const FVector ParentLocation = ParentActor->GetActorLocation();

    const float YOffset = TargetLocation.Y - ParentLocation.Y;
    const float ZOffset = TargetLocation.Z - ParentLocation.Z;

    const bool bYInRange = YOffset >= YMin && YOffset <= YMax;
    const bool bZInRange = ZOffset >= ZMin && ZOffset <= ZMax;

    return bYInRange && bZInRange;
}

void ABaseAIFighter::SetStateFromParam(int32 Param)
{
    // 기존 Set States 블루프린트 함수 로직 여기에 이식
}

void ABaseAIFighter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseAIFighter, LeftRight);
    DOREPLIFETIME(ABaseAIFighter, UpDown);
    DOREPLIFETIME(ABaseAIFighter, AbilityType);
    DOREPLIFETIME(ABaseAIFighter, Direction);
}
