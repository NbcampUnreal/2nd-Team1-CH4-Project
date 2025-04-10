#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SSTCharacter.h"
#include "Core/SmashTypes.h"
#include "Core/SmashUitilityLibrary.h"
#include "Interfaces/Interface_SmashCombat.h"
#include "SmashCharacter.generated.h"

class USmashCombatComponent;
class USmashCharacterStats;
class USmashCharacterMovementComponent;
class USmashStateSystem;
class USmashAbilitySystemComponent;
class UFXComponent;
class ASmashGameState;
class USmashGameInstance;
class ASmashPlatFighterGameMode;
class UWidgetComponent;
class UUW_HUD_CharacterInfo;
class UParticleSystem;

DECLARE_LOG_CATEGORY_EXTERN(LogSmashCharacter, Log, All);

/**
 * 대난투 스타일 게임의 기본 캐릭터 클래스
 * 네트워크 복제, 상태 관리, 오류 처리 기능이 강화됨
 */
UCLASS()
class SMASHBRAWL_API ASmashCharacter : public ASSTCharacter, public IInterface_SmashCombat
{
	GENERATED_BODY()

public:
	/** 생성자 및 기본 엔진 오버라이드 함수 */
	ASmashCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	//---------------------------------------------------------------------
	// 초기화 및 설정 함수
	//---------------------------------------------------------------------
protected:
	/** 초기화 단계 함수들 */
	UFUNCTION()
	void StartUp();

	void SetupHUDWidget();

	/** 능력 시스템 초기화 */
	void InitializeAbilitySystem();

	UFUNCTION()
	void SetUp();

	/** 게임 인스턴스 및 상태 설정 */
	void SetupGameInstance();
	void SetupGameState();
	void SetupGameMode();
	void SyncClientPlayerData();

	/** 캐릭터 외형 및 장비 설정 */
	void SetCostume();
	void AttachAbilities();
	void UpdateCamera();

public:
	/** 초기화 상태 확인 */
	UFUNCTION(BlueprintPure, Category = "Smash Character")
	bool IsInitialized() const { return bInitialized; }

	//---------------------------------------------------------------------
	// 인풋 액션 및 입력 처리
	//---------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Smash Character |Movement")
	void DodgeDelayEvent();

	UFUNCTION(BlueprintCallable, Category = "Smash Character |Movement")
	void ScreenShake(float RumbleInten, float RumbleDuration);

	UFUNCTION(BlueprintCallable, Category = "Smash Character |Movement")
	void LedgeGrab();

public:
	/** 기본 입력 처리 함수 */
	virtual void Move(const struct FInputActionValue& Value) override;
	void UpDownAxis(const FInputActionValue& InputActionValue);
	virtual void JumpOrDrop_Implementation() override;
	void ResetMoveInput(const FInputActionValue& Value);
	void BasicAttack();
	virtual void CrouchDrop_Implementation() override;
	virtual void StopCrouchDrop_Implementation() override;

	/** 입력 이벤트 처리 함수 */
	void BasicAttackPressed(const FInputActionValue& InputActionValue);
	void BasicAttackReleased(const FInputActionValue& InputActionValue);
	void SpecialAttackPressed(const FInputActionValue& InputActionValue);
	void SpecialAttackReleased(const FInputActionValue& InputActionValue);
	void TauntUpPressed(const FInputActionValue& InputActionValue);
	void TauntRightPressed(const FInputActionValue& InputActionValue);
	void TauntLeftPressed(const FInputActionValue& InputActionValue);
	void TauntDownPressed(const FInputActionValue& InputActionValue);
	void DodgePressed(const FInputActionValue& InputActionValue);
	void GrabPressed(const FInputActionValue& InputActionValue);

	/** 방향 및 입력 상태 확인 함수 */
	UFUNCTION(BlueprintCallable, Category = "Smash Character|Input")
	void UpdateDirection();

	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	float GetMoveInputValue() const { return MoveInputValue; }

	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	float GetUpDownInputValue() const { return UpDownInputValue; }

	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	bool HasMoveInput() const { return !FMath::IsNearlyZero(MoveInputValue); }

	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	bool IsMovingRight() const { return MoveInputValue > 0.0f; }

	//---------------------------------------------------------------------
	// 캐릭터 상태 및 움직임 관리
	//---------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Smash Character |Movement", Server, Reliable)
	void Launch(FVector LaunchVector, bool bXYOver, bool bZOver);
public:
	/** 상태 관리 함수 */
	void Dizzy();

	float NockBackCalculate();

	void ShieldHit(ESmashDirection NewDirection);

	void LandedAction();

	UFUNCTION()
	void OnLandedSmash(const FHitResult& Hit);

public:
	UFUNCTION(BlueprintCallable, Category="Smash Character|Movement")
	void SetMovementState(FSmashPlayerMovement SetMovement);

	/** 방향 전환 관련 함수 */
	UFUNCTION(BlueprintCallable, Category = "Smash Character|Movement")
	void SetCanFlip(bool bNewCanFlip);

	UFUNCTION(BlueprintPure, Category = "Smash Character|Movement")
	bool CanFlip() const;

	/** 상태 정보 접근자 */
	UFUNCTION(BlueprintPure, Category = "Smash Character|State")
	const FSmashPlayerStateInfo& GetStateInfo() const { return StateInfo; }

	UFUNCTION(BlueprintPure, Category = "Smash Character|State")
	FSmashPlayerMovement GetMovementInfo() const { return StateInfo.PlayerMovement; }

	UFUNCTION(BlueprintCallable, Category = "Smash Character|State")
	void SetStateInfo(const FSmashPlayerStateInfo& NewStateInfo);

	/** 복제 이벤트 핸들러 */
	UFUNCTION()
	virtual void OnRep_StateInfo();

	UFUNCTION()
	void OnRep_LastHit();

	UFUNCTION()
	void OnRep_PlayerNo();

	/** 네트워크 RPC 함수 */
	UFUNCTION(Server, Reliable)
	void Server_SetCanFlip(bool bNewCanFlip);

	UFUNCTION(Server, Reliable)
	void Server_RequestSetStateInfo(const FSmashPlayerStateInfo& NewStateInfo);

	UFUNCTION(Client, Reliable)
	void Client_UpdateStateInfo(const FSmashPlayerStateInfo& NewStateInfo);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SmashDetection();

	//---------------------------------------------------------------------
	// 전투 및 히트 처리
	//---------------------------------------------------------------------
public:
	/** 방향 및 위치 업데이트 */
	void FacingCheck();
	void UpdateLocations();

	/** 이펙트 및 피격 처리 */
	void UpdateFlashing();

	UFUNCTION(BlueprintCallable, Category = "Smash Character|Movement")
	void SpawnFeetFX();

	void TauntAction(ESmashDirection ActionDirection);

	//---------------------------------------------------------------------
	// 인터페이스 구현 - IInterface_SmashCombat
	//---------------------------------------------------------------------
public:
	virtual ESmashPlayerStates GetPlayerState_Implementation() override;
	virtual ESmashAttacks GetAttackTypes_Implementation() override;
	virtual ESmashAbilityTypes GetAbilityTypes_Implementation() override;
	virtual ESmashDirection GetDirection_Implementation() override;
	virtual int32 GetSuper_Implementation() override;
	virtual FTransform GetAbilitySpawnTransform_Implementation() override;
	virtual void SetSuper_Implementation(int32 NewSuper) override;
	virtual void SetAttacks_Implementation(ESmashAttacks NewAttacks) override;
	virtual void BufferButtons_Implementation() override;
	virtual void ClearBuffer_Implementation() override;

	//---------------------------------------------------------------------
	// 컴포넌트
	//---------------------------------------------------------------------
public:
	/** 주요 컴포넌트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smash Character|Component")
	TObjectPtr<USmashCharacterMovementComponent> SmashCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smash Character|Component")
	TObjectPtr<USmashCharacterStats> SmashCharacterStatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character|Component")
	TObjectPtr<USmashCombatComponent> SmashCombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character|Component")
	TObjectPtr<USmashStateSystem> SmashStateSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character|Component")
	TObjectPtr<USmashAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character|Component")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Character|Component")
	TObjectPtr<UFXComponent> FXComponent;

	/** 게임 인스턴스 및 상태 */
	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	TObjectPtr<USmashGameInstance> SmashGameInstance;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	TObjectPtr<ASmashGameState> SmashGameState;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	TObjectPtr<ASmashPlatFighterGameMode> SmashGameMode;

	//---------------------------------------------------------------------
	// 설정 및 입력 액션
	//---------------------------------------------------------------------
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config")
	UParticleSystem* SprintFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config")
	UParticleSystem* P_Landed;

	/** 입력 액션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Input")
	UInputAction* IA_UpDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Input")
	UInputAction* IA_BasicAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Input")
	UInputAction* IA_SpecialAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_Dodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_Grab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_TauntUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Input")
	UInputAction* IA_TauntRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Input")
	UInputAction* IA_TauntLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Input")
	UInputAction* IA_TauntDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Config|Cosmetics")
	TArray<TObjectPtr<UMaterialInstance>> TeamOptionMaterials;

	//---------------------------------------------------------------------
	// 복제 속성
	//---------------------------------------------------------------------
public:
	/** 상태 및 히트 관련 */
	UPROPERTY(ReplicatedUsing=OnRep_StateInfo, BlueprintReadWrite, Category = "Smash Character|State")
	FSmashPlayerStateInfo StateInfo;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	bool bFlashing;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	bool bIsSmashFlash;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Input")
	bool bAttackButton;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Input")
	bool bAttackButtonReleased;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Input")
	bool bSpecialAttackButton;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Input")
	bool bSpecialAttackButtonReleased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character")
	float InvonFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character")
	float SmashFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character")
	float HitFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character")
	float FreeFallFlash;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	bool bDodgeDelay = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_PlayerNo, BlueprintReadWrite, Category="Smash Character")
	int32 PlayerNo;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	ESmashCharacter Character;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	ESmashHitState HitStates;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	ESmashAbilityTypes AbilityType;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	ESmashAttacks Attacks;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	int32 Team;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	int32 Damage;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	int32 BaseKnock = 5;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	float DefaultCapsuleHeight;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	float Knockback = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	float DamRatios = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	float HitScale;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	float DodgeDelayTimer = 0.2f;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	float ProneTime = 0.0f;
	

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Cosmetics")
	UMaterialInstanceDynamic* Material;

	UPROPERTY(ReplicatedUsing=OnRep_LastHit, BlueprintReadWrite, Category="Smash Character")
	TObjectPtr<ASmashCharacter> LastHit;

	/** 입력 관련 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Smash Character|Input")
	float MoveInputValue;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Smash Character|Input")
	float UpDownInputValue;

	/** 위치 관련 */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	FVector Location;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	float YPos;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	float ZPos;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	float FootZPos;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	FVector LocationFeet;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	FVector LedgeLocation;

	/** 상태 플래그 */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bAttachedAbil = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bLedgeCooldown = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bLanded = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bDizzied = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bPushed = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bCanSmash = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bBufferdInput = true;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bBufferdDirection = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bHitRest = true;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	bool bSpiked = false;

	/** 전투 관련 */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Smash Character")
	ESmashDirection Direction;

	//---------------------------------------------------------------------
	// 비복제 상태 속성
	//---------------------------------------------------------------------
public:
	/** 히트 및 효과 관련 */
	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	bool bEnergyHit;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	bool bHitByProjectile;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	bool bReflectMode;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	bool bAbsorbMode;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	bool bJustHit;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	bool bHitButNoEffect;

	/** 전투 관련 */
	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	int32 ProjectileDamage;
	
	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	ESmashBuffer BufferMove = ESmashBuffer::Attack;

	UPROPERTY(BlueprintReadWrite, Category = "Smash Character")
	ESmashDirection BufferDirection = ESmashDirection::Up;

	/** 기타 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Character|Widget")
	TObjectPtr<UUW_HUD_CharacterInfo> UW_HUDCharacterInfo;

private:
	/** 초기화 완료 여부 추적 */
	bool bInitialized;

	/** DoOnce 헬퍼 */
	FDoOnce DoOnceMoveDirection;
	FDoOnce DoOnceUpDirection;
	FDoOnce DoOnceDownDirection;
	FDoOnce DoOnceBasicAttackPressed;
};
