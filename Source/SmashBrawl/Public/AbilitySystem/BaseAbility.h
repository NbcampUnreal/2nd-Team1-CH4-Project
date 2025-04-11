

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Core/SmashTypes.h"
#include "BaseAbility.generated.h"

class ASmashCharacter;

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
	UFUNCTION(NetMulticast, Reliable, Category = "BaseAbility|Network")
	void Multicast_AbilityStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "BaseAbility|Network")
	void BP_OnAbilityStart();
	
	UFUNCTION()
	void StartTimeline();

	UFUNCTION()
	void EndTimeline();

	UFUNCTION(BlueprintImplementableEvent)
	void ActiveLoop();

	UFUNCTION(BlueprintImplementableEvent)
	void CameraZoom(float ZoomAmount, float Duration);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_TickRep();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_SoftEnd();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_EndAbility();

	UFUNCTION(BlueprintNativeEvent, Category = "BaseAbility|Network")
	void BP_OnEndAbility();
	
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

	UFUNCTION(BlueprintCallable)
	void RemoveDamagers();

	UFUNCTION(BlueprintCallable)
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

	UFUNCTION(BlueprintCallable)
	void AnimNotifyStart(bool InDamager, FName NoteName);

	UFUNCTION(BlueprintCallable)
	void AnimNotifyEnd(bool InDamager, FName NoteName);

	UFUNCTION(BlueprintCallable)
	void DebugAbilityCall(int32 PlayerNumber, bool bDebug);

	UFUNCTION(BlueprintCallable)
	void FuncCharging();

	UFUNCTION(BlueprintCallable)
	void SpecialAttackButton();

	UFUNCTION(BlueprintCallable)
	void SetBuffer();

	UFUNCTION(BlueprintImplementableEvent)
	void Land();
	
	UFUNCTION(BlueprintImplementableEvent)
	void TakeAHit();
	
	UFUNCTION(BlueprintImplementableEvent)
	void HitByProjectile(int32 Damage, bool bIsEnergy);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShootProjectile();
	
	UFUNCTION(BlueprintCallable)
	void SetMovement(bool bCanMove);
	
	UFUNCTION(BlueprintCallable)
	void SetCanAttack(bool bCanAttack);
	
	UFUNCTION(BlueprintCallable)
	void SetJump(bool bCanJump);
	
	UFUNCTION(BlueprintCallable)
	void DodgeDelay();
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_SetFlip(bool bCanFlip);
	
	UFUNCTION(BlueprintCallable)
	void CharacterCollision(ECollisionEnabled::Type Type);
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetCollisionInfo();

	UFUNCTION(BlueprintCallable)
	void WalkOffLedge(bool bCanWalkOff);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_EndAnim(int32 InAnimNo);

	UFUNCTION(BlueprintImplementableEvent, Category = "BaseAbility|Network")
	void BP_OnEndAnim(int32 InAnimNo);
	
	UFUNCTION(BlueprintCallable)
	void ActivateOtherAbility(ABaseAbility* Ability);

	UFUNCTION(BlueprintCallable)
	void HitStates(ESmashHitState SmashHitState);

	UFUNCTION(BlueprintCallable)
	void SetAttackStage(int32 _AttackStage);

	UFUNCTION(BlueprintCallable)
	void ChangeCollisionSet();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCounter();

	UFUNCTION(BlueprintCallable)
	void AbsorbMode(bool bAbsorb);

	UFUNCTION(BlueprintCallable)
	void ReflectMode(bool bReflect);

	UFUNCTION(BlueprintCallable)
	void FullbodyLedge(bool bFullBodyLedgeGrab);

	UFUNCTION(BlueprintCallable)
	void TeleportCharacter(FVector Location, bool bSweep, bool bTeleport);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReflect();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BaseAbility|Network")
	void Multicast_StartParticles(bool bAttached, UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent,
	                              FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale,
	                              EAttachLocation::Type LocationType, bool bAutoDestroy);

	UFUNCTION(BlueprintCallable)
	void SetGravity(int32 Settings, float CustomValue);

	UFUNCTION(BlueprintCallable)
	void HealPlayer(int32 HealAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void SpecialButtonUp();

	UFUNCTION(BlueprintImplementableEvent)
	void SpecialButtonDown();

	UFUNCTION(BlueprintCallable)
	void EndAllNonChargedAbilities(ABaseAbility* Caller);

	
	UFUNCTION(BlueprintImplementableEvent)
	void AbilityEvent();

	UFUNCTION(BlueprintCallable)
	void Charge(bool InChargeing);

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseAbility|Timeline")
	class UTimelineComponent* MainTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseAbility|Timeline")
	UCurveFloat* CurveFloat;

	FOnTimelineFloat TimelineUpdate;
	FOnTimelineEvent TimelineFinished;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Charge")
	float ChargeRatio;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Charge")
	float ChargeMax;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Charge")
	float ChargeLevel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Charge")
	float ChargeRate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Charge")
	bool bCharging;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Charge")
	bool bCharged;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bActive;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	TObjectPtr<ASmashCharacter> Parent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	FVector ParentLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bInputButter;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bIsUse;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bLanding;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	int32 AttackStage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	TArray<ACharacter*> ChildDamagers;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bActivateCollision;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	TObjectPtr<UDataTable> AttackData;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	TSet<FName> AttackContain;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bDamager;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	ESmashFaceing Direction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	TArray<AActor*> DamagerInfo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	int32 DamIndex;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	TArray<FTransform> BoxTransforms;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bDontEndOnLedge;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	bool bStale;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	ESmashFaceing BufferDirection;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	ESmashBuffer Buffer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility")
	int32 StailLevel;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|DoOnce")
	bool bBufferClosed = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|DoOnce")
	bool bLandClosed = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|DoOnce")
	bool bTakeAHitClosed = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|DoOnce")
	bool bHitByProjectileClosed = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	int32 CollisionSet;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<FTransform> CollisionBoxTransforms;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<int32> TableNo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<ESmashShape> CollisionShape;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<ESmashCollisionType> CollisionType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<FName> CollisionSockets;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<int32> CollisionSets;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<FVector> AttackLocations;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<USoundBase*> HitSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<UParticleSystem*> HitParticle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<FName> Attacks;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<bool> GrabBoxes;
	TObjectPtr<AActor> HitBox;// class Damager
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	int32 CollisionSetIndex;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Collisions")
	TArray<int32> CollisionSetArray;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Animation")
	int32 AnimNo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Animation")
	TArray<UAnimMontage*> Animations;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Projectile")
	TSubclassOf<AActor> Projectile;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Projectile")
	FTransform ProjectileTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Setup")
	bool bShowAttackStageOnly;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BaseAbility|Setup")
	int32 EditorAttackStage;
	
	FTimerHandle ReShieldTimerHandle;
};