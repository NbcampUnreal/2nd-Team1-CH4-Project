// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseAbility.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseAbility : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	virtual void Multicast_AbilityStart();

	void EndAbility()
	{
	}

	UFUNCTION()
	void StartTimeline();

	UFUNCTION()
	void EndTimeline();

	UFUNCTION()
	virtual void ActiveLoop();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_TickRep();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_SoftEnd();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_EndAbility();

	UFUNCTION()
	void Reset();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_LandLeg(UAnimMontage* LandAnimation, float PlayRate, float StartPosition, FName StartSection,
	                       int32 InAnimNo);

	UFUNCTION()
	void ActivateDamagers();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_AddDamagersClient(ACharacter* InParent);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Server_AddDamagersServer(ACharacter* InParent);

	UFUNCTION()
	void FuncDamageBoxes(ACharacter* InParent);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Server_SpawnProjectile(TSubclassOf<AActor> ProjectileClass,
	                            FTransform SpawnTransform,
	                            float Lifetime,
	                            float YSpeed,
	                            float ZSpeed,
	                            float Gravity,
	                            FName AttackName,
	                            float DamageMultiply);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_Throw(FName Name);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_ReShield();

	UFUNCTION()
	void RemoveDamagers();

	UFUNCTION()
	void FlipOnStick();

	UFUNCTION(BlueprintCallable)
	void PlayAnimation(int32 InAnimNo);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_PlayAnimationClient(int32 InAnimNo, UAnimMontage* InMontageToPlay);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Server_PlayAnimationServer(int32 InAnimNo, UAnimMontage* InMontageToPlay);

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void AnimNotifyStart(bool InDamager, FName NoteName);

	UFUNCTION()
	void AnimNotifyEnd(bool InDamager, FName NoteName);

	UFUNCTION()
	void DebugAbilityCall(int32 PlayerNumber, bool bDebug);

	UFUNCTION()
	void FuncCharging();

	UFUNCTION()
	void SpecialAttackButton();

	UFUNCTION()
	void SetBuffer();

	UFUNCTION()
	virtual void Land();
	UFUNCTION()
	virtual void TakeAHit();
	UFUNCTION()
	virtual void HitByProjectile(int32 Damage, bool bIsEnergy);
	virtual void ShootProjectile();
	UFUNCTION()
	void SetMovement(bool bCanMove);
	UFUNCTION()
	void SetCanAttack(bool bCanAttack);
	UFUNCTION()
	void SetJump(bool bCanJump);
	UFUNCTION()
	void SetFlip(bool bCanFlip);
	UFUNCTION()
	void DodgeDelay();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_SetFlip(bool bCanFlip);
	UFUNCTION()
	void CharacterCollision();
	UFUNCTION()
	virtual void GetCollisionInfo();
	UFUNCTION()
	void WalkOffLedge(bool bCanWalkOff);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_EndAnim(int32 InAnimNo);
	UFUNCTION()
	void ActivateOtherAbility(ABaseAbility* Ability);
	UFUNCTION()
	void HitStates();
	UFUNCTION()
	void SetAttackStage(int32 _AttackStage);
	UFUNCTION()
	void ChangeCollisionSet();
	UFUNCTION()
	virtual void OnCounter();
	UFUNCTION()
	void AbsorbMode(bool bAbsorb);
	UFUNCTION()
	void ReflectMode(bool bReflect);
	UFUNCTION()
	void TeleportCharacter(FVector Location, bool bSweep, bool bTeleport);
	UFUNCTION()
	virtual void OnReflect();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_StartParticles(bool bAttached, UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent,
	                              FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale,
	                              EAttachLocation::Type LocationType, bool bAutoDestroy);
	UFUNCTION()
	void SetGravity(int32 Settings, float CustomValue);
	UFUNCTION()
	void HealPlayer(int32 HealAmount);
	UFUNCTION()
	virtual void SpecialButtonUp();
	UFUNCTION()
	virtual void SpecialButtonDown();
	UFUNCTION()
	virtual void AbilityEvent();

	UFUNCTION(BlueprintCallable,Category="BaseAbility")
	void Charge(bool InChargeing) { bCharging = InChargeing; }

	UPROPERTY()
	class UTimelineComponent* MainTimeline;

	FOnTimelineFloat TimelineUpdate;
	FOnTimelineEvent TimelineFinished;

	UPROPERTY()
	UCurveFloat* CurveFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAbility|Charge")
	float ChargeRatio;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAbility|Charge")
	float ChargeMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAbility|Charge")
	float ChargeLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAbility|Charge")
	float ChargeRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAbility|Charge")
	bool bCharging;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAbility|Charge")
	bool bCharged;


	UPROPERTY()
	bool bActive;

public:
	UPROPERTY(BlueprintReadWrite,Category="BaseAbility")
	TObjectPtr<ACharacter> Parent;

	UPROPERTY()
	FVector ParentLocation;

	UPROPERTY()
	bool bInputButter;
	UPROPERTY()
	bool bBufferClosed = true;
	UPROPERTY()
	bool bLandClosed = false;
	UPROPERTY()
	bool bTakeAHitClosed = false;
	UPROPERTY()
	bool bHitByProjectileClosed = false;
	UPROPERTY()
	bool bIsUse;
	UPROPERTY()
	bool bLanding;
	UPROPERTY()
	int32 AttackStage;
	UPROPERTY()
	int32 CollisionSetIndex;
	UPROPERTY()
	TArray<int32> CollisionSetArray;
	UPROPERTY()
	TArray<ACharacter*> ChildDamagers; //BPDamagerInfo �迭�� �����ؾ���
	UPROPERTY()
	int32 AnimNo;
	UPROPERTY()
	int32 CollisionSet;
	UPROPERTY()
	bool bActivateCollision;
	
	UPROPERTY()
	UDataTable* AttackData;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BaseAbility")
	TArray<UAnimMontage*> Animations;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BaseAbility")
	TSet<FName> AttackContain;
	UPROPERTY()
	bool bDamager;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAbility")
	TSubclassOf<AActor> Projectile;
	
	FTimerHandle ReShieldTimerHandle;
};
