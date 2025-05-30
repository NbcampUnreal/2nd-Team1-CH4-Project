#include "Monster/Character/BaseAIFighter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "AbilitySystem/HitBox/SmashAIDamagerManager.h"
#include "Monster/LevelObjects/RespawnLocation.h"

ABaseAIFighter::ABaseAIFighter()
{
    bReplicates = true;
    GetCharacterMovement()->SetIsReplicated(true);
    SpawnDamagerManager = CreateDefaultSubobject<USmashAIDamagerManager>(FName("DamageManager"));
    PrimaryActorTick.bCanEverTick = true;
}

void ABaseAIFighter::BeginPlay()
{
    Super::BeginPlay();

    CurrentHP = MaxHP;

    if (HasAuthority())
    {
        Multicast_PlaySpawnSound();
    }
}

bool ABaseAIFighter::bHitConditions()
{
    return true;
}

void ABaseAIFighter::TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection)
{
    if (!HasAuthority()) return;

    CurrentHP -= static_cast<float>(DamageAmount);
    CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);

    UE_LOG(LogTemp, Warning, TEXT("AI 피격: 현재 HP = %.1f / %.1f"), CurrentHP, MaxHP);

    Multicast_PlayHitMontage(bIsRightDirection);

    if (CurrentHP <= 0.f)
    {
        Die();
    }
}

void ABaseAIFighter::PlayRandomAttackMontage()
{
    if (HasAuthority())
    {
        Multicast_PlayRandomAttackMontage();
        if (SpawnDamagerManager)
        {
            Damagers = SpawnDamagerManager->SpawnDamagerAll();
        }
    }
}

void ABaseAIFighter::Multicast_PlayRandomAttackMontage_Implementation()
{
    const float Rand = FMath::FRand();

    if (Rand < 0.2f && AttackMontage_Uppercut)
    {
        GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage_Uppercut);
    }
    else if (Rand < 0.6f && AttackMontage_Left)
    {
        GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage_Left);
    }
    else if (AttackMontage_Right)
    {
        GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage_Right);
    }
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

bool ABaseAIFighter::IsTargetInRange(AActor* TargetActor, AActor* ParentActor, float XMin, float XMax, float ZMin, float ZMax)
{
    if (!TargetActor || !ParentActor)
    {
        return false;
    }

    const FVector TargetLocation = TargetActor->GetActorLocation();
    const FVector ParentLocation = ParentActor->GetActorLocation();

    const float XOffset = TargetLocation.X - ParentLocation.X;
    const float ZOffset = TargetLocation.Z - ParentLocation.Z;

    const bool bXInRange = XOffset >= XMin && XOffset <= XMax;
    const bool bZInRange = ZOffset >= ZMin && ZOffset <= ZMax;

    return bXInRange && bZInRange;
}

void ABaseAIFighter::SetStateFromParam(int32 Param)
{
}

//void ABaseAIFighter::TakeDamage(float DamageAmount)
//{
//    if (!HasAuthority()) return;
//
//    CurrentHP -= DamageAmount;
//    CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
//
//    UE_LOG(LogTemp, Warning, TEXT("AI 피격: 현재 HP = %.1f"), CurrentHP);
//
//    if (CurrentHP <= 0.f)
//    {
//        Die();
//    }
//}

void ABaseAIFighter::Die()
{
    if (!bActive) return;

    bActive = false;

    // 몽타주 중지
    if (GetMesh() && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
    }

    if (HasAuthority())
    {
        Multicast_PlayDeathSound();
    }
    
    PlayDeathMontage();
    SetLifeSpan(3.0f);
}

void ABaseAIFighter::PlayDeathMontage()
{
    if (HasAuthority())
    {
        Multicast_PlayDeathMontage();
    }
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

void ABaseAIFighter::Multicast_PlayDeathMontage_Implementation()
{
    if (DeathMontage && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
    }
}

void ABaseAIFighter::Multicast_PlayHitMontage_Implementation(bool bIsRightDirection)
{
    UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
    if (AnimInst)
    {
        if (bIsRightDirection && KnockMontage_Backward)
        {
            AnimInst->Montage_Play(KnockMontage_Backward);
        }
        else if (!bIsRightDirection && KnockMontage_Forward)
        {
            AnimInst->Montage_Play(KnockMontage_Forward);
        }
    }
}

void ABaseAIFighter::Multicast_PlaySpawnSound_Implementation()
{
    if (SpawnSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
    }
}

void ABaseAIFighter::Multicast_PlayDeathSound_Implementation()
{
    if (DeathSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
    }
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
