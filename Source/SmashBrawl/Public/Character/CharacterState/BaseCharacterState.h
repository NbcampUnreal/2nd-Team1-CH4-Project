#pragma once

#include "CoreMinimal.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseCharacterState.generated.h"

class USmashStateSystem;
class ASmashCharacter;
class USmashAction;

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
	void InitState(USmashStateSystem* InStateSystem);

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

	// 액션 타입으로 액션 찾기
	UFUNCTION(BlueprintCallable, Category = "Character State")
	USmashAction* FindActionByType(ESmashActionType ActionType);

	// 액션 실행
	UFUNCTION(BlueprintCallable, Category = "Character State")
	bool ExecuteActionByType(ESmashActionType ActionType);

	// 액션 리스트 반환
	UFUNCTION(BlueprintCallable, Category = "Character State")
	TArray<USmashAction*> GetActions() const { return Actions; }

	UFUNCTION(BlueprintPure, Category = "Character State")
	bool IsCurrentPlayerState(ESmashPlayerStates CheckState);

protected:
	// 액션 초기화
	void InitActions();

public:
	// 이 상태의 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character State")
	ESmashPlayerStates PlayerState;

	// 소유 상태 시스템 참조
	UPROPERTY(BlueprintReadWrite, Category="Character State")
	TObjectPtr<USmashStateSystem> OwnerStateSystem;

	// 소유 캐릭터 참조
	UPROPERTY(BlueprintReadWrite, Category="Character State")
	TObjectPtr<ASmashCharacter> OwnerCharacter;

	// 이 상태에서 사용 가능한 액션 클래스 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character State")
	TArray<TSubclassOf<USmashAction>> ActionClasses;

	// 생성된 액션 인스턴스 목록
	UPROPERTY(BlueprintReadOnly, Category="Character State")
	TArray<TObjectPtr<USmashAction>> Actions;
};
