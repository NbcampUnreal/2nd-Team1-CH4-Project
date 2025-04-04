#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SSTCharacter.h"
#include "Core/SmashTypes.h"
#include "SmashCharacter.generated.h"

class ASmashPlatFighterGameMode;
class UFXComponent;
class AShield;
class ASmashGameState;
class USmashGameInstance;
class ABaseAbility;
class UWidgetComponent;
class UStateSystem;

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

	// 방향 체크 및 업데이트
	void FacingCheck();

	// 이전에 없던 함수들을 구현
	void SprintCheck();

	//이동 입력 값이 없으면


	void SmashDetection();
	void UpdateLocations();
	void UpdateFlashing();
	void UpdateWalkRun();
	void UpdateStates();

public:
	// 이동 입력값 접근 함수
	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	float GetMoveInputValue() const { return MoveInputValue; }

	// 이동 입력 존재 여부 확인 함수
	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	bool HasMoveInput() const { return !FMath::IsNearlyZero(MoveInputValue); }

	// 이동 방향 확인 함수 (오른쪽 방향인지)
	UFUNCTION(BlueprintPure, Category = "Smash Character|Input")
	bool IsMovingRight() const { return MoveInputValue > 0.0f; }

	virtual void Move(const struct FInputActionValue& Value) override;
	void ResetMoveInput(const FInputActionValue& Value);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Smash Character|Config|Cosmetics")
	TArray<TObjectPtr<UMaterialInstance>> TeamOptionMaterials;

	// BaseCharacterState에서 옮겨온 StateInfo
	UPROPERTY(ReplicatedUsing=OnRep_StateInfo, BlueprintReadWrite, Category="Smash Character|State")
	FSmashPlayerStateInfo StateInfo;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	bool bAttachedAbil = false;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerNo, BlueprintReadWrite, Category="Smash Character")
	int32 PlayerNo;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	ESmashCharacter Character;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Smash Character")
	int32 Team;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character|Cosmetics")
	UMaterialInstanceDynamic* Material;

	UPROPERTY(ReplicatedUsing=OnRep_LastHit, BlueprintReadWrite, Category="Smash Character")
	TObjectPtr<ASmashCharacter> LastHit;

	UPROPERTY(BlueprintReadWrite, Category="Smash Character")
	AShield* Shield;

protected:
	// 현재 이동 입력값 (네트워크 복제됨)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Smash Character|Input")
	float MoveInputValue;

private:
	// 초기화 완료 여부 추적
	bool bInitialized;

	// 기본 FX 속성 추가
	UPROPERTY(EditAnywhere, Category="Smash Character|FX")
	float FlashDuration;

	// 캐릭터 상태 추적 변수들
	float FlashTimeRemaining;
	bool bIsFlashing;
};

