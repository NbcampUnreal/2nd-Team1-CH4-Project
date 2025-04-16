// #pragma once
//
// #include "CoreMinimal.h"
// #include "FollowCameraComponent.h"
// #include "SmashCameraComponent.generated.h"
//
// class ASmashCharacter;
//
// // 카메라 모드 열거형
// UENUM(BlueprintType)
// enum class ECameraMode : uint8
// {
//     Default UMETA(DisplayName = "Default"), // 기본 모드 (개인 플레이어 추적)
//     Group UMETA(DisplayName = "Group")      // 그룹 모드 (모든 플레이어 표시)
// };
//
// /**
//  * 대난투 스타일 게임의 카메라 컴포넌트.
//  * 플레이어들을 추적하고 상황에 맞는 카메라 모드를 지원합니다.
//  * 각 플레이어는 독립적으로 카메라를 제어할 수 있으며, 그룹 모드에서는 모든 캐릭터가 화면에 표시됩니다.
//  */
// UCLASS()
// class SMASHBRAWL_API USmashCameraComponent : public UFollowCameraComponent
// {
// 	GENERATED_BODY()
//
// public:
// 	USmashCameraComponent();
//
// 	virtual void BeginPlay() override;
// 	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
// 	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
// 	virtual void CacheOwner() override;
//
// 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 카메라 모드 관련 함수
// public:
// 	/**
//      * 카메라 모드를 설정합니다. 각 클라이언트에서 독립적으로 작동합니다.
// 	 * @param NewMode - 설정할 카메라 모드
// 	 * @param BlendTime - 전환 시간(초)
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Mode")
// 	void SetCameraMode(ECameraMode NewMode, float BlendTime = 1.0f);
//
// 	/**
// 	 * 카메라 모드에 따른 설정을 적용합니다.
// 	 * @param Mode - 적용할 카메라 모드
// 	 * @param BlendTime - 전환 시간(초)
// 	 */
// 	void ApplyCameraModeSettings(ECameraMode Mode, float BlendTime);
//
// 	/** 현재 카메라 모드를 반환합니다. */
// 	UFUNCTION(BlueprintPure, Category = "Camera|Mode")
// 	ECameraMode GetCameraMode() const { return CurrentCameraMode; }
//
// 	/**
// 	 * 이 카메라가 로컬 플레이어의 것인지 확인합니다.
// 	 * @return 로컬 플레이어가 소유한 카메라인 경우 true
// 	 */
// 	UFUNCTION(BlueprintPure, Category = "Camera|Mode")
// 	bool IsLocalPlayerCamera() const;
//
// 	/**
// 	 * 카메라 초기화 - 로컬 플레이어 여부에 따라 다른 모드 설정
// 	 * @param bGroupModeForHost - 호스트가 그룹 모드를 사용할지 여부
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Mode")
// 	void InitializeCameraForPlayer(bool bGroupModeForHost = true);
//
//     // 타겟 관리 함수들
//
// 	/** 추적 중인 타겟 액터 배열을 반환합니다. */
// 	UFUNCTION(BlueprintPure, Category = "Camera|Targets")
// 	const TArray<AActor*>& GetTargetActors() const { return TargetActors; }
//
// 	/**
// 	 * 타겟 액터를 추가합니다.
// 	 * @param TargetActor - 추가할 타겟 액터
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
// 	void AddTargetActor(AActor* TargetActor);
//
// 	/**
// 	 * 타겟 액터를 제거합니다.
// 	 * @param TargetActor - 제거할 타겟 액터
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
// 	void RemoveTargetActor(AActor* TargetActor);
//
// 	/** 모든 타겟을 제거합니다. */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
// 	void ClearTargets();
//
// 	/**
// 	 * 메인 타겟을 설정합니다. 기본 카메라 모드에서 주로 추적하는 대상입니다.
// 	 * @param NewMainTarget - 설정할 메인 타겟 액터
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
// 	void SetMainTarget(AActor* NewMainTarget);
//
// 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 카메라 효과 함수
//
// 	/**
// 	 * 카메라 흔들림 효과를 적용합니다.
// 	 * @param Intensity - 흔들림 강도
// 	 * @param Duration - 지속 시간(초)
// 	 * @param Falloff - 감쇠율
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
// 	void ShakeCamera(float Intensity, float Duration, float Falloff = 1.0f);
//
// 	/**
// 	 * 모든 타겟이 화면에 들어오도록 자동으로 줌을 조정합니다.
// 	 * @param BlendTime - 전환 시간(초)
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
// 	void ZoomToTargets(float BlendTime = 1.0f);
//
// 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 플레이어 관리 함수
// 	/**
// 	 * 플레이어를 카메라 시스템에 등록합니다.
// 	 * @param Player - 등록할 플레이어 캐릭터
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Players")
// 	void RegisterPlayer(ASmashCharacter* Player);
//
// 	/**
// 	 * 플레이어를 카메라 시스템에서 등록 해제합니다.
// 	 * @param Player - 등록 해제할 플레이어 캐릭터
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Players")
// 	void UnregisterPlayer(ASmashCharacter* Player);
//
// 	/**
// 	 * 자동 카메라 모드 활성화 여부를 설정합니다.
// 	 * @param bEnabled - 활성화 여부
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Settings")
// 	void SetAutoCameraEnabled(bool bEnabled);
//
// 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 마스터 카메라 함수
//
// 	/** 이 카메라를 마스터 카메라로 설정합니다. 다른 플레이어의 독립성은 유지됩니다. */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Master")
// 	void BecomesMasterCamera();
//
// 	/** 마스터 카메라 상태를 해제합니다. */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|Master")
// 	void StopBeingMasterCamera();
//
// 	/** 이 카메라가 마스터 카메라인지 확인합니다. */
// 	UFUNCTION(BlueprintPure, Category = "Camera|Master")
// 	bool IsMasterCamera() const { return bIsMasterCamera; }
//
// 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 뷰 타겟 함수
//
// 	/**
// 	 * 플레이어 컨트롤러의 뷰 타겟을 설정합니다.
// 	 * @param PlayerController - 대상 플레이어 컨트롤러
// 	 * @param BlendTime - 전환 시간(초)
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|ViewTarget")
// 	void SetPlayerViewTarget(APlayerController* PlayerController, float BlendTime = 1.0f);
//
// 	/**
// 	 * 모든 등록된 플레이어의 뷰 타겟을 설정합니다.
// 	 * @param BlendTime - 전환 시간(초)
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "Camera|ViewTarget")
// 	void SetAllPlayersViewTarget(float BlendTime = 1.0f);
//
// 	// 네트워크 RPC 함수
//
// 	/** 서버에 마스터 카메라 설정 요청 */
// 	UFUNCTION(Server, Reliable)
// 	void Server_BecomesMasterCamera();
//
// 	/** 모든 클라이언트에 마스터 카메라 설정 알림 */
// 	UFUNCTION(NetMulticast, Reliable)
// 	void Multicast_BecomesMasterCamera();
//
// protected:
//     /** 그룹 모드 카메라 업데이트 로직 */
// 	void UpdateGroupCamera(float DeltaTime);
//
//     /** 카메라 흔들림 효과 업데이트 */
// 	void UpdateCameraShake(float DeltaTime);
//     
//     /** 최적의 카메라 뷰 계산 */
// 	void UpdateOptimalCameraView();
//     
//     /** 마스터 카메라 추가 업데이트 */
// 	void UpdateMasterCamera();
//
//     /** 최적의 줌 거리 계산 */
//     float CalculateOptimalZoomDistance() const;
//
// public:
// 	UFUNCTION(Client, Reliable)
// 	void Client_AddTargetActor(AActor* TargetActor);
//
// protected:
// 	UFUNCTION()
// 	void OnRep_RegisteredPlayers();
//
// public:
// 	// 기본 설정
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config")
//     float MinGroupZoomDistance = 800.0f; // 그룹 모드에서 최소 줌 거리
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config")
//     float MaxGroupZoomDistance = 2000.0f; // 그룹 모드에서 최대 줌 거리
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config")
//     float TargetPadding = 100.0f; // 타겟 주변 여백 (화면 가장자리 여유 공간)
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config")
//     float ZoomSmoothingFactor = 5.0f; // 줌 부드러움 계수 (값이 클수록 더 빠르게 줌)
//
// 	// 등록된 플레이어 목록
// 	UPROPERTY(ReplicatedUsing=OnRep_RegisteredPlayers)
//     TArray<ASmashCharacter*> RegisteredPlayers;
//
// 	// 자동 카메라 활성화 여부
// 	UPROPERTY(Replicated)
//     bool bAutoCameraEnabled;
//
// private:
// 	// 현재 카메라 모드 - 복제 속성 제거 (각 클라이언트가 독립적으로 설정)
// 	UPROPERTY(VisibleInstanceOnly, Category = "Camera|State")
//     ECameraMode CurrentCameraMode;
//
//     // 이전 카메라 모드
//     UPROPERTY()
//     ECameraMode PreviousCameraMode;
//
// 	// 대상 액터 목록
// 	UPROPERTY()
//     TArray<AActor*> TargetActors;
//
// 	// 메인 타겟 (기본 추적 대상)
// 	UPROPERTY()
//     AActor* MainTarget;
//
// 	// 마스터 카메라 여부
// 	UPROPERTY(Replicated)
//     bool bIsMasterCamera;
//
// 	// 카메라 흔들림 효과 처리 변수
//     bool bIsCameraShaking;
//     float ShakeIntensity;
//     float ShakeDuration;
//     float ShakeTimer;
//     float ShakeFalloff;
//     FVector OriginalCameraLocation;
//
// 	// 리드 오프셋 보간
//     FVector CurrentGroupPositionOffset;
//
//     // 디버깅 로그 출력 설정
//     UPROPERTY(EditAnywhere, Category = "Smash Camera|Debug")
//     bool bEnableDebugLogging = false;
// };