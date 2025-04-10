#include "Monster/Character/BaseAIFighter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Monster/LevelObjects/RespawnLocation.h"

ABaseAIFighter::ABaseAIFighter()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = true;
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

void ABaseAIFighter::StartRecovery()
{
    bRecoverMode = true;
    bActive = false;

    // 리스폰 위치 찾기
    TArray<AActor*> RespawnPoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnLocation::StaticClass(), RespawnPoints);

    if (RespawnPoints.Num() > 0)
    {
        RespawnLocation = RespawnPoints[0]->GetActorLocation();
        MoveTowardRespawn();

        // 점프 가능 여부에 따라 리셋 시도
        if (CanJump())
        {
            ResetRecovery();
        }
    }
}

void ABaseAIFighter::MoveTowardRespawn()
{
    FVector MyLocation = GetActorLocation();
    FVector MoveDirection = (RespawnLocation - MyLocation).GetSafeNormal2D();
    AddMovementInput(MoveDirection, 1.0f);
}

void ABaseAIFighter::ResetRecovery()
{
    if (bResetRecoveryInProgress) return;

    bResetRecoveryInProgress = true;

    GetWorldTimerManager().SetTimer(RecoveryHandle, this, &ABaseAIFighter::FinishRecovery, 2.0f, false);
}

void ABaseAIFighter::FinishRecovery()
{
    bRecoverMode = false;
    bResetRecoveryInProgress = false;
    bActive = true;
}

void ABaseAIFighter::FlipDirection()
{
    FVector Scale = GetActorScale3D();
    Scale.X *= -1;
    SetActorScale3D(Scale);
}

bool ABaseAIFighter::IsActive() const
{
    return bActive;
}

bool ABaseAIFighter::IsInMoveAwayMode() const
{
    return bMoveAwayMode;
}

bool ABaseAIFighter::IsCPU() const
{
    return bIsCPU;
}

bool ABaseAIFighter::IsOffStage() const
{
    return !bOverGround;
}

bool ABaseAIFighter::IsFacingRight() const
{
    return GetActorForwardVector().X > 0;
}
