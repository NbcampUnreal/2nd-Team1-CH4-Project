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

void ABaseAIFighter::SetStateFromParam(int32 Param)
{
    // TODO: 기존 Set States 블루프린트 함수 로직 여기에 이식
}

void ABaseAIFighter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseAIFighter, LeftRight);
    DOREPLIFETIME(ABaseAIFighter, UpDown);
    DOREPLIFETIME(ABaseAIFighter, AbilityType);
    DOREPLIFETIME(ABaseAIFighter, Direction);
}
