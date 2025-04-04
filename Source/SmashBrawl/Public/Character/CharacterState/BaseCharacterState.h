#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseCharacterState.generated.h"

class UStateSystem;
class ASmashCharacter;


/**
 * 캐릭터 상태를 나타내는 기본 클래스
 * UObject를 상속받기 때문에 네트워크 복제에 직접 사용하지 않고,
 * 상태 ID와 데이터만 복제하는 방식으로 변경
 */
UCLASS()
class SMASHBRAWL_API UBaseCharacterState : public UObject
{
	GENERATED_BODY()

public:
	UBaseCharacterState();

public:
	// 상태 초기화 - StateSystem과 연결
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void InitState(UStateSystem* InStateSystem);

	// 상태 진입 시 호출
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void EnterState();

	// 상태 종료 시 호출
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void ExitState();

	// 상태 업데이트 시 호출
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	void TickState();

	// 이 상태로 진입 가능한지 확인
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character State")
	bool CanState();

	// 상태 ID 반환
	UFUNCTION(BlueprintPure, Category = "Character State")
	ESmashPlayerStates GetPlayerState() const;

	// 캐릭터의 상태 정보에 접근
	UFUNCTION(BlueprintPure, Category = "Character State")
	const FSmashPlayerStateInfo& GetStateInfo() const;

	// 캐릭터의 상태 정보 설정 - 서버 권한 확인 추가
	UFUNCTION(BlueprintCallable, Category = "Character State")
	void SetStateInfo(const FSmashPlayerStateInfo& NewStateInfo);

protected:
	// 이 상태의 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character State")
	ESmashPlayerStates PlayerState;

	// 소유 상태 시스템 참조
	UPROPERTY(BlueprintReadWrite, Category="Character State")
	TObjectPtr<UStateSystem> OwnerStateSystem;

	// 소유 캐릭터 참조
	UPROPERTY(BlueprintReadWrite, Category="Character State")
	TObjectPtr<ASmashCharacter> OwnerCharacter;
};
