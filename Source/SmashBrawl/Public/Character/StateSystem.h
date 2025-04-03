// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "StateSystem.generated.h"


class UAbilityTypeManager;
class UCharacterStateManager;
class UBaseCharacterState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API UStateSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UStateSystem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="State System")
	UBaseCharacterState* FindState(EPlayerStates FindToState);

	// 서버에서 실행할 상태 변경 함수
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="State System")
	void Server_ChangeState(EPlayerStates NewState);

	// 클라이언트에서 호출할 상태 변경 함수
	UFUNCTION(BlueprintCallable, Category="State System")
	void ChangeState(EPlayerStates NewState);

	UFUNCTION(BlueprintCallable, Category="State System")
	EPlayerStates GetCurrentState() const;

	// 디버깅 도우미 함수
	UFUNCTION(BlueprintCallable, Category="State System")
	void DebugStateSystem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State System")
	TArray<TSubclassOf<UBaseCharacterState>> InitStates;

	// 네트워크 복제가 되도록 수정
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "State System")
	TArray<TObjectPtr<UBaseCharacterState>> States;

	// 네트워크 복제가 되도록 수정
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "State System")
	TObjectPtr<UBaseCharacterState> CurrentState;

	// 초기 상태 설정을 위한 속성 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State System")
	EPlayerStates DefaultState;

private:
	// 내부 상태 변경 함수
	void Internal_ChangeState(EPlayerStates NewState);
};
