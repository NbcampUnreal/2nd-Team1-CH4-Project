#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SSTCharacter.h"
#include "Core/SmashTypes.h"
#include "Core/SmashUitilityLibrary.h"
#include "SmashCharacter.generated.h"

class USmashCharacterStats;
class ASmashPlatFighterGameMode;
class UFXComponent;
class AShield;
class ASmashGameState;
class USmashGameInstance;
class ABaseAbility;
class UWidgetComponent;
class UStateSystem;
class UParticleSystem;

DECLARE_LOG_CATEGORY_EXTERN(LogSmashCharacter, Log, All);

/**
 * 대난투 스타일 게임의 기본 캐릭터 클래스
 * 네트워크 복제, 상태 관리, 오류 처리 기능이 강화됨
 */
UCLASS()
class SMASHBRAWL_API ASmashCharacter : public ASSTCharacter
{
	GENERATED_BODY()

public:
	ASmashCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 초기화 함수 개선
	UFUNCTION()
	void StartUp();

	UFUNCTION()
	void SetUp();

	void SetCostume();

	void AttachAbilities();

	void AttachShield();

	void UpdateCamera();

public:
	virtual void Tick(float DeltaSeconds) override;

	void FacingCheck();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SmashDetection();

	void UpdateLocations();

	void UpdateFlashing();

	UFUNCTION(BlueprintCallable, Category = "Smash Character|Movement")
	void SpawnFeetFX();

	void TauntAction(ESmashDirection ActionDirection);

public:
	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	float GetMoveInputValue() const { return MoveInputValue; }

	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	bool HasMoveInput() const { return !FMath::IsNearlyZero(MoveInputValue); }

	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	bool IsMovingRight() const { return MoveInputValue > 0.0f; }

public:
	void Dizzy();

	float NockBackCalculate();

	void ShieldHit(ESmashDirection NewDirection);

	void LandedAction();

	UFUNCTION()
	void OnLandedSmash(const FHitResult& Hit);

public:
	virtual void Move(const struct FInputActionValue& Value) override;

	void UpDownAxis(const FInputActionValue& InputActionValue);

	virtual void JumpOrDrop_Implementation() override;

	void ResetMoveInput(const FInputActionValue& Value);

	void BasicAttack();


	void BasicAttackPressed(const FInputActionValue& InputActionValue);
	void BasicAttackReleased(const FInputActionValue& InputActionValue);

	void SpecialAttackPressed(const FInputActionValue& InputActionValue);
	void SpecialAttackReleased(const FInputActionValue& InputActionValue);

	void TauntUpPressed(const FInputActionValue& InputActionValue);
	void TauntRightPressed(const FInputActionValue& InputActionValue);
	void TauntLeftPressed(const FInputActionValue& InputActionValue);
	void TauntDownPressed(const FInputActionValue& InputActionValue);

public:
	UFUNCTION(BlueprintCallable, Category="Smash Character|Movement")
	void SetMovementState(FSmashPlayerMovement SetMovement);

	// 방향 전환만 활성화/비활성화 - 권한 체크 강화
	UFUNCTION(BlueprintCallable, Category = "Smash Character|Movement")
	void SetCanFlip(bool bNewCanFlip);

	// 현재 방향 전환 가능 상태 확인
	UFUNCTION(BlueprintPure, Category = "Smash Character|Movement")
	bool CanFlip() const;

	// 서버 RPC 함수
	UFUNCTION(Server, Reliable)
	void Server_SetCanFlip(bool bNewCanFlip);

	// 클라이언트 RPC 함수 추가 - 상태 정보 변경 전파
	UFUNCTION(Client, Reliable)
	void Client_UpdateStateInfo(const FSmashPlayerStateInfo& NewStateInfo);

public:
	// StateInfo 접근자 추가
	UFUNCTION(BlueprintPure, Category = "Smash Character|State")
	const FSmashPlayerStateInfo& GetStateInfo() const { return StateInfo; }

	UFUNCTION(BlueprintPure, Category = "Smash Character|State")
	FSmashPlayerMovement GetMovementInfo() const{return StateInfo.PlayerMovement;}

	// StateInfo 설정 함수 추가 - 권한 체크 개선
	UFUNCTION(BlueprintCallable, Category = "Smash Character|State")
	void SetStateInfo(const FSmashPlayerStateInfo& NewStateInfo);

	// 상태 정보 변경 시 호출될 함수
	UFUNCTION()
	virtual void OnRep_StateInfo();

	// 초기화 상태 확인
	UFUNCTION(BlueprintPure, Category = "Smash Character")
	bool IsInitialized() const { return bInitialized; }

	// LastHit 복제 시 호출되는 함수 추가
	UFUNCTION()
	void OnRep_LastHit();

	// PlayerNo 복제 시 호출되는 함수 추가
	UFUNCTION()
	void OnRep_PlayerNo();

public:
	/** 커스텀 캐릭터 이동 컴포넌트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Smash Character|Component")
	TObjectPtr<class USmashCharacterMovementComponent> SmashCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Smash Character|Component")
	TObjectPtr<USmashCharacterStats> SmashCharacterStatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character|Component")
	TObjectPtr<class UStateSystem> StateSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character|Component")
	TObjectPtr<class USmashAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character|Component")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smash Character|Component")
	TObjectPtr<UFXComponent> FXComponent;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	TObjectPtr<USmashGameInstance> SmashGameInstance;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	TObjectPtr<ASmashGameState> SmashGameState;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	TObjectPtr<ASmashPlatFighterGameMode> SmashGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config")
	TSubclassOf<AShield> ShieldClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config")
	FName ShieldSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config")
	UParticleSystem* SprintFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config")
	UParticleSystem* P_Landed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_UpDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_BasicAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_SpecialAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_TauntUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_TauntRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_TauntLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Input")
	UInputAction* IA_TauntDown;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Cosmetics")
	TArray<TObjectPtr<UMaterialInstance>> TeamOptionMaterials;

	UPROPERTY(ReplicatedUsing=OnRep_StateInfo, BlueprintReadWrite, Category="Smash Character|State")
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

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	bool bAttachedAbil = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	bool bLedgeCooldown = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	bool bLanded = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	bool bDizzied = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	bool bPushed = false;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerNo, BlueprintReadWrite, Category="Smash Character")
	int32 PlayerNo;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	ESmashCharacter Character;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	ESmashHitState HitStates;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	ESmashAbilityTypes AbilityType;

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

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Cosmetics")
	UMaterialInstanceDynamic* Material;

	UPROPERTY(ReplicatedUsing=OnRep_LastHit, BlueprintReadWrite, Category="Smash Character")
	TObjectPtr<ASmashCharacter> LastHit;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	AShield* Shield;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	ESmashDirection Direction;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Smash Character|Input")
	float MoveInputValue;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Smash Character|Input")
	float UpDownInputValue;

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

private:
	// 초기화 완료 여부 추적
	bool bInitialized;

	FDoOnce DoOnceMoveDirection;
	FDoOnce DoOnceUpDirection;
	FDoOnce DoOnceDownDirection;
	FDoOnce DoOnceBasicAttackPressed;
};
