#pragma once

#include "CoreMinimal.h"
#include "SmashUitilityLibrary.generated.h"

/**
 * 블루프린트의 DoOnce 매크로와 동일한 기능을 수행하는 C++ 구조체
 * 특정 코드 블록이 정확히 한 번만 실행되도록 보장합니다.
 * 멀티플레이어 게임에서 사용 가능하도록 리플리케이션을 고려했습니다.
 */
USTRUCT(BlueprintType)
struct FDoOnce
{
	GENERATED_BODY()

private:
	// 이미 실행되었는지 여부를 추적하는 변수
	bool bHasBeenExecuted;

public:
	// 기본 생성자
	FDoOnce() : bHasBeenExecuted(false)
	{
	}

	// StartClosed 옵션을 가진 생성자
	FDoOnce(bool bStartClosed) : bHasBeenExecuted(bStartClosed)
	{
	}

	// 실행 상태 확인
	bool HasBeenExecuted() const
	{
		return bHasBeenExecuted;
	}

	// 실행 메서드 - 콜백 함수를 받는 템플릿 버전
	template <typename FunctorType>
	bool ExecuteIfNot(FunctorType&& Functor)
	{
		if (!bHasBeenExecuted)
		{
			bHasBeenExecuted = true;
			Functor();
			return true;
		}
		return false;
	}

	// 간단한 실행 메서드 - 한 번만 true를 반환
	bool Execute()
	{
		if (!bHasBeenExecuted)
		{
			bHasBeenExecuted = true;
			return true;
		}
		return false;
	}

	// 리셋 메서드
	void Reset()
	{
		bHasBeenExecuted = false;
	}
};
