#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Core/SmashTypes.h"
#include "SmashAction.generated.h"

class UBaseCharacterState;
class ASmashCharacter;

/**
 * 상태 내에서 실행할 수 있는 액션 클래스
 * 각 액션은 특정 상태와 연결되며, 실행 조건과 로직을 포함
 */
UCLASS(Abstract, Blueprintable)
class SMASHBRAWL_API USmashAction : public UObject
{
	GENERATED_BODY()

public:
	USmashAction();

	// 액션 초기화
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	void InitAction(UBaseCharacterState* InOwnerState);

	// 액션 실행
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	bool Execute();

	// 액션 실행 가능 여부 확인
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	bool CanExecute();

	// 액션 타입 반환
	UFUNCTION(BlueprintPure, Category="Action")
	ESmashActionType GetActionType() const { return ActionType; }

	// 액션 소유 캐릭터 반환
	UFUNCTION(BlueprintPure, Category="Action")
	ASmashCharacter* GetOwnerCharacter() const;


protected:
	// 액션 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	ESmashActionType ActionType;

	// 소유 상태 참조
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TObjectPtr<UBaseCharacterState> OwnerState;
};
