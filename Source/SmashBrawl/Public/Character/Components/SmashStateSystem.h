#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "SmashStateSystem.generated.h"

class UBaseCharacterState;

SMASHBRAWL_API DECLARE_LOG_CATEGORY_EXTERN(LogSmashState, Log, All);

/**
 * 캐릭터 상태를 관리하는 시스템
 * 네트워크 복제를 개선하고 상태 전환 안정성을 높임
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashStateSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashStateSystem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 상태 ID로 상태 객체 찾기
	UFUNCTION(BlueprintCallable, Category="State System")
	UBaseCharacterState* FindState(ESmashPlayerStates FindToState);

	// 상태 변경 시도 함수 (조건 체크 포함, 결과 반환)
	UFUNCTION(BlueprintCallable, Category="State System")
	bool TryChangeState(ESmashPlayerStates NewState, bool bCheckCanState = true);

	// 서버에서 상태 변경 시도 함수
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="State System")
	void Server_TryChangeState(ESmashPlayerStates NewState, bool bCheckCanState = true);

	// 현재 상태 ID 반환
	UFUNCTION(BlueprintCallable, Category="State System")
	ESmashPlayerStates GetCurrentState() const;

	// 현재 상태 변경 시 호출될 함수
	UFUNCTION()
	void OnRep_CurrentStateID();

	// 디버깅 도우미 함수
	UFUNCTION(BlueprintCallable, Category="State System")
	void DebugStateSystem();

protected:
	// 단순 상태 변경 함수 (조건 체크 없음)
	UFUNCTION(BlueprintCallable, Category="State System")
	void ChangeState(ESmashPlayerStates NewState);

	// 서버에서 실행할 상태 변경 함수
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="State System")
	void Server_ChangeState(ESmashPlayerStates NewState);

public:
	// 초기화할 상태 클래스 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State System")
	TArray<TSubclassOf<UBaseCharacterState>> InitStates;

	// 생성된 상태 객체 목록
	UPROPERTY(BlueprintReadOnly, Category = "State System")
	TArray<TObjectPtr<UBaseCharacterState>> States;

	// 현재 상태 객체 (복제되지 않음, CurrentStateID로부터 설정됨)
	UPROPERTY(BlueprintReadOnly, Category = "State System")
	TObjectPtr<UBaseCharacterState> CurrentState;

	// 현재 상태 ID (복제됨)
	UPROPERTY(ReplicatedUsing=OnRep_CurrentStateID, BlueprintReadOnly, Category = "State System")
	ESmashPlayerStates CurrentStateID;

	// 초기 상태 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State System")
	ESmashPlayerStates DefaultState;

private:
	// 초기화 완료 여부
	bool bInitialized;
};
