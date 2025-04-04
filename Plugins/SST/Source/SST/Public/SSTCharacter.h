#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSTCharacter.generated.h"

/**
 * 기본 ACharacter를 확장하여 다음 기능을 제공:
 * - 대시, 벽 슬라이드 등을 지원하는 커스텀 캐릭터 이동 컴포넌트
 * - 캐릭터를 따라다니며 CameraTrigger 오브젝트를 통해 수정 가능한 FollowCamera 컴포넌트
 * - 좌/우 이동, 점프, 대시, 웅크리기, 플랫폼 낙하 등을 위한 입력 바인딩
 * 
 * 캐릭터 이동은 USSTCharacterMovementComponent가 전적으로 담당합니다.
 */
UCLASS(config=Game)
class SST_API ASSTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** 
	 * 생성자 - 기본 서브오브젝트 생성 및 초기화
	 * @param ObjectInitializer - 오브젝트 초기화에 사용되는 FObjectInitializer 인스턴스
	 */
	ASSTCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	/** 
	 * 이동 입력에 대한 처리
	 * @param Value - 입력 액션 값, 좌우 이동의 방향과 강도 결정
	 */
	void Move(const struct FInputActionValue& Value);

	/** 
	 * 웅크리기/낙하 입력에 대한 처리
	 * 캐릭터가 웅크리게 하거나, 웅크린 상태에서 플랫폼을 통과하도록 함
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void CrouchDrop();

	/** 
	 * 웅크리기/낙하 입력 해제 시 호출
	 * 캐릭터가 웅크린 상태에서 일어나도록 함
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void StopCrouchDrop();

	/** 
	 * 점프 버튼이 눌렸을 때 호출되며, 상황에 따라 점프 또는 낙하 기능 수행
	 * 웅크린 상태라면 플랫폼 통과, 아니면 점프 실행
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void JumpOrDrop();

	/** 
	 * 대시 입력에 대한 처리
	 * 캐릭터가 현재 방향으로 빠르게 대시하도록 함
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Dash();

protected:
	// APawn 인터페이스
	/** 
	 * 플레이어 입력 컴포넌트 설정
	 * 모든 입력 액션을 바인딩함
	 * @param PlayerInputComponent - 설정할 입력 컴포넌트
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 
	 * 게임 시작 시 또는 액터 생성 시 호출
	 * 입력 매핑 컨텍스트 추가 등 초기화 작업 수행
	 */
	virtual void BeginPlay();

public:
	/**
	 * 커스텀 캐릭터 이동 컴포넌트 반환
	 * @return USSTCharacterMovementComponent* - 이 캐릭터의 커스텀 이동 컴포넌트
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE class USSTCharacterMovementComponent* GetSSTCharacterMovement() const { return SSTCharacterMovementComponent; }

	/**
	 * 현재 캐릭터가 대시할 수 있는지 확인
	 * 블루프린트에서 오버라이드하여 커스텀 대시 조건 확인 가능
	 * @return bool - 대시 가능 여부
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "Movement")
	bool CanDash() const;

	/**
	 * 점프 버튼 해제 시 호출
	 * 점프 상태를 리셋하고 점프 키 홀드 중지
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void ReleaseJump();

	/**
	 * 캐릭터가 웅크릴 수 있는지 확인
	 * 커스텀 이동 모드를 고려하여 기본 CanCrouch 함수를 오버라이드함
	 * @return bool - 웅크리기 가능 여부
	 */
	virtual bool CanCrouch() const override;

	/**
	 * 점프 상태 중지
	 * 내부 점프 관련 플래그 초기화
	 */
	virtual void StopJumping() override;

	/**
	 * 점프 상태 리셋
	 * 커스텀 이동 모드(벽 슬라이드, 벽 점프, 대시 등)에 맞게 오버라이드됨
	 */
	virtual void ResetJumpState() override;

	/**
	 * 착지 시 호출
	 * 점프 카운트 초기화 등 착지 관련 처리 수행
	 * @param Hit - 착지 위치 및 표면 정보
	 */
	virtual void Landed(const FHitResult& Hit) override;

	/**
	 * 점프 입력 체크
	 * 기본 Character 클래스의 동작을 변경하여 점프 키 누르고 있는 동안 자동 재점프 방지
	 * @param DeltaTime - 프레임 간 경과 시간
	 */
	virtual void CheckJumpInput(float DeltaTime) override;

	/**
	 * 이 액터와 그 자식들을 무시하는 충돌 쿼리 파라미터 반환
	 * @return FCollisionQueryParams - 액터 자신과 모든 자식을 무시하는 충돌 쿼리 파라미터
	 */
	virtual FCollisionQueryParams GetIgnoreSelfParams() const;

public:
	/** 커스텀 캐릭터 이동 컴포넌트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USSTCharacterMovementComponent> SSTCharacterMovementComponent;

	/** 카메라 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFollowCameraComponent> FollowCamera;

	/** 매핑 컨텍스트 - 입력 액션과 입력 장치의 매핑을 정의 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	/** 점프 입력 액션 - 점프 또는 플랫폼 낙하 명령에 사용 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	/** 웅크리기/낙하 입력 액션 - 캐릭터가 웅크리거나 플랫폼을 통과하도록 함 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchDropAction;

	/** 이동 입력 액션 - 좌우 이동 제어 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	/** 대시 입력 액션 - 캐릭터의 빠른 대시 기능 실행 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;

	/** 플랫폼을 통과한 후 점프 입력이 다시 활성화되기까지의 시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float DropThroughPlatformJumpLockout = .2f;

private:
	/** 점프 버튼이 계속 눌려있는 상태인지 추적하는 플래그 */
	bool IsJumpStale = false;
};
