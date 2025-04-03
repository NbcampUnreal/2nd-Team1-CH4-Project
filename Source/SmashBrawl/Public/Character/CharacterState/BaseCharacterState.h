#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseCharacterState.generated.h"

class UStateSystem;
class ABaseSSTCharacter;

// 상태별 추가 정보를 위한 구조체 (네트워크 복제를 위함)
USTRUCT(BlueprintType)
struct FPlayerStateInfo
{
	GENERATED_BODY()

	// 액션 가능 여부 플래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	bool bCanJump = true;

	// // 디버프 효과 정보
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	// bool bHasSlowDebuff = false;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	// bool bHasBlindDebuff = false;
	//
	// // 디버프 지속 시간
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	// float DebuffDuration = 0.0f;
};

UCLASS()
class SMASHBRAWL_API UBaseCharacterState : public UObject
{
	GENERATED_BODY()

public:
	UBaseCharacterState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void InitState(UStateSystem* InStateSystem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void EnterState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void ExitState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void TickState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	bool CanState();

	// 상태 정보 변경 시 호출될 함수
	UFUNCTION()
	virtual void OnRep_StateInfo();

public:
	UFUNCTION(BlueprintPure, Category = "Character State")
	EPlayerStates GetPlayerState() const;

	// 상태 정보 getter/setter
	UFUNCTION(BlueprintPure, Category = "Character State")
	const FPlayerStateInfo& GetStateInfo() const { return StateInfo; }

	UFUNCTION(BlueprintCallable, Category = "Character State")
	void SetStateInfo(const FPlayerStateInfo& NewStateInfo);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character State")
	EPlayerStates PlayerState;

	UPROPERTY(BlueprintReadWrite, Category="Character State")
	TObjectPtr<UStateSystem> OwnerStateSystem;

	UPROPERTY(BlueprintReadWrite, Category="Character State")
	TObjectPtr<ABaseSSTCharacter> OwnerCharacter;

	// 상태별 추가 정보 (복제되도록 설정)
	UPROPERTY(ReplicatedUsing=OnRep_StateInfo, BlueprintReadWrite, Category="Character State")
	FPlayerStateInfo StateInfo;
};
