#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSTCharacterMovementComponent.generated.h"

/**
 * 기본 CharacterMovementComponent를 확장하여 다음과 같은 커스텀 이동 기능을 제공합니다:
 *   - 벽 슬라이딩(Wallsliding) - 벽을 타고 미끄러지는 기능
 *   - 벽 점프(Walljumping) - 벽에서 반동을 이용해 점프하는 기능
 *   - 대시(Dashing) - 빠르게 가속하는 기능
 *   - 일방통행 플랫폼(OneWayPlatform)을 통과하는 기능
 *
 * 또한 180도 좌/우 부드러운 회전 기능을 제공하며 속도를 설정할 수 있습니다.
 *
 * 이 기능들은 네트워크 플레이를 지원하도록 리플리케이션 기능이 구현되어 있습니다.
 */

// 대시 동작은 블루프린트에서 델리게이트를 통해 커스터마이즈 가능합니다
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformDashDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndDashDelegate);

// 커스텀 캐릭터 이동 모드 열거형
UENUM(BlueprintType)
enum ESSTCharacterMovementMode
{
	FSSTMOVE_NONE UMETA(Hidden),
	FSSTMOVE_WALLSLIDING UMETA(DisplayName = "Wallsliding"),
	FSSTMOVE_DASHING UMETA(DisplayName = "Dashing"),
	FSSTMOVE_MAX UMETA(Hidden)
};

/**
 * 사이드 스크롤 게임 캐릭터를 위한 커스텀 캐릭터 이동 컴포넌트입니다.
 * 벽 슬라이딩, 벽 점프, 대시 등 플랫포머 게임에서 필요한 이동 메커니즘을 구현합니다.
 * 네트워크 환경에서도 정확한 이동 동기화를 위해 움직임 예측 및 리플리케이션 기능을 포함합니다.
 */
UCLASS()
class SST_API USSTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/* 상태 데이터 리플리케이션을 위한 SavedMove 클래스 */
	class FSavedMove_SSTCharacter : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		/** 캐릭터가 오른쪽을 보고 있는지 여부를 저장 */
		bool SavedFacingRight;

		/** 대시 요청 상태를 저장 */
		bool SavedWantsToDash;

		/** 플랫폼 통과 요청 상태를 저장 */
		bool SavedWantsToPlatformDrop;

		/** 새로운 이동과 결합 가능한지 확인 - 네트워크 대역폭 최적화에 사용 */
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

		/** 상태 초기화 */
		virtual void Clear() override;

		/** 압축된 플래그 반환 - 네트워크 전송 최적화에 사용 */
		virtual uint8 GetCompressedFlags() const override;

		/** 캐릭터의 상태를 저장 */
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;

		/** 저장된 상태를 캐릭터에 적용 */
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	/* 커스텀 SavedMove 클래스를 사용하기 위한 NetworkPredictionData 오버라이드 */
	class FNetworkPredictionData_Client_SSTCharacter : public FNetworkPredictionData_Client_Character
	{
	public:
		/** 생성자 - 클라이언트 이동 컴포넌트를 참조하여 초기화 */
		FNetworkPredictionData_Client_SSTCharacter(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		/** 새로운 이동 객체 할당 - 커스텀 SavedMove 클래스 사용 */
		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	/** 커스텀 예측 데이터를 반환 - 네트워크 이동 예측 시스템에 필요 */
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

public:
	/** 생성자 - 기본 세팅 초기화 */
	USSTCharacterMovementComponent();

	/** 리플리케이션 속성 등록 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/** 게임 시작 시 호출됨 - 초기화 작업 수행 */
	virtual void BeginPlay() override;

protected:
	/** 압축된 플래그에서 상태 업데이트 - 네트워크 동기화에 사용 */
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	/** 이동 모드가 변경될 때 호출됨 */
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode);

	/** 이동 전에 캐릭터 상태 업데이트 - 벽 슬라이딩, 대시 등 확인 */
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	/** 새 물리 프레임 시작 - 회전 처리 등 */
	virtual void StartNewPhysics(float deltaTime, int32 Iterations) override;

	/** 커스텀 물리 처리 - 벽 슬라이딩, 대시 등의 물리 처리 */
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	/** 현재 최대 이동 속도 반환 - 이동 모드에 따라 다름 */
	virtual float GetMaxSpeed() const override;

	/** 현재 최대 감속 반환 - 이동 모드에 따라 다름 */
	virtual float GetMaxBrakingDeceleration() const override;

	/** 점프 시도 가능 여부 확인 - 벽 점프, 대시 취소 점프 등 고려 */
	virtual bool CanAttemptJump() const override;

	/** 착지 처리 - 대시 카운트 리셋 등 */
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

private:
	/** 벽 슬라이딩 시작 */
	void EnterWallslide();

	/** 벽 슬라이딩 종료 */
	void ExitWallslide();

	/** 벽 슬라이딩 물리 처리 */
	void PhysWallslide(float DeltaTime, int32 Iterations);

	/** 벽 충돌 감지 트레이스 */
	bool GetWallslideWall(TArray<FHitResult>& Hits) const;

	/** 바닥 충돌 감지 트레이스 */
	bool GetWallslideFloor(TArray<FHitResult>& Hits) const;

	/** 벽이 캡슐 높이보다 높은지 확인 */
	bool CheckWallAtLeastCapsuleHeight(const FHitResult& Hit);

	/** 주어진 벽이 슬라이딩에 적합한지 확인 */
	bool IsEligibleWallForSliding(FHitResult& Hit);

	/** 플레이어가 바라보는 방향으로 입력하고 있는지 확인 */
	bool IsHeadedForwards() const;

	/** 벽 슬라이딩 아래에 유효한 바닥이 있는지 확인하는 헬퍼 함수 */
	bool GetValidFloorBeneath(FHitResult& Hit);

	/** 벽 점프 수행 */
	void PerformWallJump();

	/** 대시 시작 */
	void EnterDash();

	/** 대시 종료 */
	void ExitDash(bool DashCancelled = false);

	/** 대시 물리 처리 */
	void PhysDash(float DeltaTime, int32 Iterations);

	/** 대시 수행 */
	void PerformDash();

	/** 플레이어가 일방통행 플랫폼 위에 서 있는지 확인 */
	class AOneWayPlatform* FindOneWayPlatform();

public:
	/** 커스텀 이동 모드 활성화를 위한 오버라이드 */
	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override;

	/** 점프 수행 - 벽 점프, 대시 취소 등 처리 */
	virtual bool DoJump(bool bReplayingMoves, float DeltaTime) override;

	/** 현재 상태에서 웅크릴 수 있는지 확인 */
	virtual bool CanCrouchInCurrentState() const override;

	/** 캐릭터가 현재 오른쪽을 향하고 있는지 여부(또는 그 방향으로 회전 중인지) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	FORCEINLINE bool IsFacingRight() const { return FacingRight; }

	/** 소유 캐릭터에 의해 BeginPlay()에서 호출됨 */
	void SetFacingRight(bool bFacingRight);

	/** 캐릭터가 현재 지정된 커스텀 이동 모드인지 확인 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool IsCustomMovementMode(ESSTCharacterMovementMode Mode) const;

	/** 캐릭터가 벽 슬라이딩 중이거나 방금 벽에서 떠나 벽 점프가 가능한지 여부 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool CanWalljump() const;

	/** 180도 회전 시작. 캐릭터가 이미 회전 중이면 false 반환 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool RequestTurnAround();

	/** 벽 슬라이딩 시작 시 호출. 블루프린트에서 확장 가능 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void OnBeginWallslide();

	/** 벽 슬라이딩 종료 시 호출. 블루프린트에서 확장 가능 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void OnEndWallslide();

	/** 캐릭터가 현재 대시할 수 있는지 여부 반환 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool CanDash() const;

	/** 대시 시작 시 호출. 블루프린트에서 확장 가능 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void OnBeginDash();

	/** 대시 종료 시 호출. 블루프린트에서 확장 가능 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void OnEndDash();

	/** 소유 캐릭터가 마지막으로 일방통행 플랫폼을 통과한 시간 반환 */
	FORCEINLINE float GetLastDropThroughPlatformTime() const { return LastDropThroughPlatformTime; }

	/** 서버에서 복제된 벽 슬라이딩 상태 처리 */
	UFUNCTION()
	virtual void OnRep_IsWallsliding();

	/** 서버에서 복제된 대시 상태 처리 */
	UFUNCTION()
	virtual void OnRep_IsDashing();


	//--------------------------------------------------------------------- Field Members
protected:
	/** 이 컴포넌트를 소유한 캐릭터 */
	UPROPERTY(Transient)
	TObjectPtr<class ASSTCharacter> SSTCharacterOwner;

	/** 완전히 회전하는 데 걸리는 시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float TurnAroundSeconds = 0.12f;

	// 벽 슬라이드 파라미터
	/** 캐릭터가 벽 슬라이딩 가능한지 여부 결정 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool CanWallSlide = true;

	/** 캐릭터가 벽을 타고 미끄러지는 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideSpeed = 200.0f;

	/** (0-1) 아날로그 입력이 벽 슬라이딩을 유지하기 위해 x축 방향으로 얼마나 정확해야 하는지 제어 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideInputAngleStrictness = 0.2f;

	/** (0-1) 값이 낮을수록 벽이 완벽한 수직에서 어느 정도 벗어나더라도 벽 슬라이딩 가능 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideAllowableWallAngleStrictness = .8f;

	/** 캐릭터가 벽에서 얼마나 떨어져서 슬라이딩하는지 거리(단위) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideDistanceFromWall = 5.0f;

	/** 착지하여 벽 슬라이딩을 종료하기 전 바닥으로부터의 최소 높이(단위) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideMinFloorHeight = 5.0f;

	/** 캐릭터가 벽 슬라이딩에서 점프할 때의 힘. 0으로 설정하면 벽 점프 비활성화 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideJumpOffForce = 1400.0f;

	/** 캐릭터가 벽 슬라이딩에서 점프할 때의 각도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideJumpAngle = 45.0f;

	/** 벽 점프 후 일반 점프 버튼이 이중 점프에 사용되기 전 대기 시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideJumpInputDisableSeconds = 0.2f;

	/** 벽 슬라이딩 해제 후 낙하가 시작된 순간부터, 점프가 여전히 벽 점프로 간주되는 버퍼 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WallslideReleaseWalljumpBufferSeconds = 0.2f;

	/** 벽 점프 간 최소 시간 간격. WallslideReleaseWalljumpBufferSeconds보다 커야 함 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalljumpCooldownSeconds = 0.5f;

	/** false로 설정하면 대시 비활성화 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool DashEnabled = true;

	/** 캐릭터가 착지 전 공중에서 수행할 수 있는 대시 횟수. 음수면 제한 없음 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	int MaxDashesBeforeLanding = 1;

	/** 대시 거리(단위) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DashDistance = 2000;

	/** 대시 지속 시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DashDurationSeconds = .2;

	/** 대시 간 쿨다운 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DashCooldownSeconds = .7;

	/** 대시 중 적용되는 마찰력. 블루프린트에서 수정 가능(예: 물속에 있을 때) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DashFriction = 0.0f;

	/** true면 플레이어가 대시 중간에 방향을 바꿀 수 있음(대시 방향 변경) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool CanTurnWhileDashing = false;

	/** true면 플레이어가 대시 중간에 점프하여 즉시 대시 종료 가능 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool CanJumpCancelDash = true;

	/** 아래쪽을 향한 경사면에서 대시할 때, true면 캐릭터가 경사면을 따라 내려가고, false면 수평 방향으로 이동 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool DashFollowsDownwardSlopes = true;

public:
	/** true로 설정하면 다음 업데이트에서 대시 시도 */
	UPROPERTY(Category = Movement, VisibleInstanceOnly, BlueprintReadOnly)
	bool WantsToDash = false;

	/** 캐릭터가 현재 대시 중인지 여부를 나타내는 플래그. 캐릭터 이동에 의해 설정됨 */
	UPROPERTY(BlueprintReadOnly, replicatedUsing = OnRep_IsDashing, Category = Character)
	uint32 IsDashing : 1;

	/** 블루프린트에서 할당 가능한 대시 구현 */
	UPROPERTY(BlueprintAssignable)
	FPerformDashDelegate PerformDashDelegate;
	UPROPERTY(BlueprintAssignable)
	FEndDashDelegate EndDashDelegate;

	/** 캐릭터가 현재 벽 슬라이딩 중인지 여부를 나타내는 플래그. 캐릭터 이동에 의해 설정됨 */
	UPROPERTY(BlueprintReadOnly, replicatedUsing = OnRep_IsWallsliding, Category = Character)
	uint32 IsWallsliding : 1;

	/** true로 설정하면 다음 업데이트에서 플랫폼 통과 시도 */
	UPROPERTY(Category = Movement, VisibleInstanceOnly, BlueprintReadOnly)
	bool WantsToPlatformDrop = false;

private:
	/** 캐릭터가 오른쪽을 향하고 있는지 여부 */
	UPROPERTY(replicated)
	bool FacingRight;

	/** 이동 입력이 활성화되어 있는지 여부 */
	bool MoveInputsEnabled = true;

	/** 캐릭터가 회전 중인지 여부 */
	bool Turning = false;

	/** 최근에 벽 슬라이딩을 했는지 여부 */
	bool HasRecentlyWallSlid = false;

	/** 최근에 벽 점프를 했는지 여부 */
	bool HasRecentlyWalljumped = false;

	/** 마지막 벽 점프 시간 */
	float LastWallJumpTime = 0;

	/** 마지막으로 벽 슬라이딩이 끝나고 낙하가 시작된 시간 */
	float LastWallslideFallingReleaseTime = 0;

	/** 마지막 벽이 캐릭터의 상대적 오른쪽에 있었는지 여부 */
	bool LastWallWasRelativeRight;

	/** 현재 착지 전 남은 대시 횟수 */
	uint8 CurrentDashesBeforeLanding = 0;

	/** 마지막 대시 시간 */
	float LastDashTime = 0;

	/** 마지막으로 플랫폼을 통과한 시간 */
	float LastDropThroughPlatformTime = 0.0f;
};
