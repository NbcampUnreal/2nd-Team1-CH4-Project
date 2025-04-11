#pragma once

#include "CoreMinimal.h"
#include "FollowCameraComponent.h"
#include "SmashCameraComponent.generated.h"

class ASmashCharacter;
class ACameraActor;

// 카메라 모드 열거형
UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	Group UMETA(DisplayName = "Group"), // 그룹 모드 (모든 플레이어 표시)
	Default UMETA(DisplayName = "Default"), // 기본 모드 (개인 플레이어 추적)
	Custom UMETA(DisplayName = "Custom") // 커스텀 모드 (외부 카메라 사용)
};

// 카메라 프리셋 구조체
USTRUCT(BlueprintType)
struct FCameraPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomDistance = 800.0f; // 카메라 줌 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LeadMaxDistance = 100.0f; // 최대 리드 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LeadSpeed = 2.0f; // 리드 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool ZLock = true; // Z축 고정 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZLockHeight = 0.0f; // Z축 고정 높이

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector LocationOffset = FVector::ZeroVector; // 위치 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector RotationOffset = FVector::ZeroVector; // 회전 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BlendTime = 1.0f; // 전환 시간
};

/**
 * 대난투 스타일 게임의 카메라 컴포넌트.
 * 플레이어들을 추적하고 상황에 맞는 카메라 모드를 지원합니다.
 * 각 플레이어는 독립적으로 카메라를 제어할 수 있으며, 그룹 모드에서는 모든 캐릭터가 화면에 표시됩니다.
 */
UCLASS()
class SMASHBRAWL_API USmashCameraComponent : public UFollowCameraComponent
{
	GENERATED_BODY()

public:
	USmashCameraComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CacheOwner() override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 카메라 모드 관련 함수
public:
	/**
     * 카메라 모드를 설정합니다. 각 클라이언트에서 독립적으로 작동합니다.
	 * @param NewMode - 설정할 카메라 모드
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Mode")
	void SetCameraMode(ECameraMode NewMode, float BlendTime = 1.0f);

	/**
	 * 카메라 모드에 따른 설정을 적용합니다.
	 * @param Mode - 적용할 카메라 모드
	 * @param BlendTime - 전환 시간(초)
	 */
	void ApplyCameraModeSettings(ECameraMode Mode, float BlendTime);

	/** 현재 카메라 모드를 반환합니다. */
	UFUNCTION(BlueprintPure, Category = "Camera|Mode")
	ECameraMode GetCameraMode() const { return CurrentCameraMode; }

	/**
	 * 이 카메라가 로컬 플레이어의 것인지 확인합니다.
	 * @return 로컬 플레이어가 소유한 카메라인 경우 true
	 */
	UFUNCTION(BlueprintPure, Category = "Camera|Mode")
	bool IsLocalPlayerCamera() const;

	/**
	 * 카메라 초기화 - 로컬 플레이어 여부에 따라 다른 모드 설정
	 * @param bGroupModeForHost - 호스트가 그룹 모드를 사용할지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Mode")
	void InitializeCameraForPlayer(bool bGroupModeForHost = true);

	// 타겟 관리 함수

	/** 추적 중인 타겟 액터 배열을 반환합니다. */
	UFUNCTION(BlueprintPure, Category = "Camera|Targets")
	const TArray<AActor*>& GetTargetActors() const { return TargetActors; }

	/**
	 * 타겟 액터를 추가합니다.
	 * @param TargetActor - 추가할 타겟 액터
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
	void AddTargetActor(AActor* TargetActor);

	/**
	 * 타겟 액터를 제거합니다.
	 * @param TargetActor - 제거할 타겟 액터
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
	void RemoveTargetActor(AActor* TargetActor);

	/** 모든 타겟을 제거합니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
	void ClearTargets();

	/**
	 * 메인 타겟을 설정합니다. 기본 카메라 모드에서 주로 추적하는 대상입니다.
	 * @param NewMainTarget - 설정할 메인 타겟 액터
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Targets")
	void SetMainTarget(AActor* NewMainTarget);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 카메라 효과 함수

	/**
	 * 카메라 흔들림 효과를 적용합니다.
	 * @param Intensity - 흔들림 강도
	 * @param Duration - 지속 시간(초)
	 * @param Falloff - 감쇠율
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
	void ShakeCamera(float Intensity, float Duration, float Falloff = 1.0f);

	/**
	 * 모든 타겟이 화면에 들어오도록 자동으로 줌을 조정합니다.
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
	void ZoomToTargets(float BlendTime = 1.0f);

	/**
	 * 정의된 카메라 프리셋으로 전환합니다.
	 * @param Preset - 적용할 카메라 프리셋
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
	void TransitionToPreset(const FCameraPreset& Preset);

	/**
	 * 트리거 영역에 들어갈 때 호출됩니다. Custom 모드로 전환하고 이전 모드를 저장합니다.
	 * @param Preset - 적용할 카메라 프리셋
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Trigger")
	void EnterCustomMode(const FCameraPreset& Preset, float BlendTime = 1.0f);

	/**
	 * 트리거 영역에서 나갈 때 호출됩니다. 이전에 저장된 모드로 돌아갑니다.
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Trigger")
	void ExitCustomMode(float BlendTime = 1.0f);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 플레이어 관리 함수
	/**
	 * 플레이어를 카메라 시스템에 등록합니다.
	 * @param Player - 등록할 플레이어 캐릭터
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Players")
	void RegisterPlayer(ASmashCharacter* Player);

	/**
	 * 플레이어를 카메라 시스템에서 등록 해제합니다.
	 * @param Player - 등록 해제할 플레이어 캐릭터
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Players")
	void UnregisterPlayer(ASmashCharacter* Player);

	/**
	 * 자동 카메라 모드 활성화 여부를 설정합니다.
	 * @param bEnabled - 활성화 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Settings")
	void SetAutoCameraEnabled(bool bEnabled);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 마스터 카메라 함수

	/** 이 카메라를 마스터 카메라로 설정합니다. 다른 플레이어의 독립성은 유지됩니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera|Master")
	void BecomesMasterCamera();

	/** 마스터 카메라 상태를 해제합니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera|Master")
	void StopBeingMasterCamera();

	/** 이 카메라가 마스터 카메라인지 확인합니다. */
	UFUNCTION(BlueprintPure, Category = "Camera|Master")
	bool IsMasterCamera() const { return bIsMasterCamera; }

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 뷰 타겟 함수

	/**
	 * 플레이어 컨트롤러의 뷰 타겟을 설정합니다.
	 * @param PlayerController - 대상 플레이어 컨트롤러
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|ViewTarget")
	void SetPlayerViewTarget(APlayerController* PlayerController, float BlendTime = 1.0f);

	/**
	 * 모든 등록된 플레이어의 뷰 타겟을 설정합니다.
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|ViewTarget")
	void SetAllPlayersViewTarget(float BlendTime = 1.0f);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 커스텀 카메라 함수

	/**
	 * 커스텀 카메라 액터를 설정합니다.
	 * @param CustomCamera - 설정할 카메라 액터
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Custom")
	void SetCustomCameraActor(ACameraActor* CustomCamera, float BlendTime = 1.0f);

	/**
	 * 기본 뷰로 리셋합니다.
	 * @param BlendTime - 전환 시간(초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Custom")
	void ResetToDefaultView(float BlendTime = 1.0f);

	// 네트워크 RPC 함수

	/** 서버에 마스터 카메라 설정 요청 */
	UFUNCTION(Server, Reliable)
	void Server_BecomesMasterCamera();

	/** 모든 클라이언트에 마스터 카메라 설정 알림 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BecomesMasterCamera();

	/** 서버에 커스텀 카메라 설정 요청 */
	UFUNCTION(Server, Reliable)
	void Server_SetCustomCameraActor(ACameraActor* CustomCamera, float BlendTime);

	/** 모든 클라이언트에 커스텀 카메라 설정 알림 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCustomCameraActor(ACameraActor* CustomCamera, float BlendTime);

private:
	// 카메라 모드별 업데이트 함수
	void UpdateDefaultCamera(float DeltaTime);
	void UpdateGroupCamera(float DeltaTime);
	void UpdateCustomCamera(float DeltaTime);

	// 유틸리티 함수
	float CalculateOptimalZoomDistance() const;
	void UpdateCameraShake(float DeltaTime);
	void UpdateOptimalCameraView();
	void UpdateMasterCamera();

public:
	UFUNCTION(Client, Reliable)
	void Client_AddTargetActor(AActor* TargetActor);
	
protected:
	UFUNCTION()
	void OnRep_RegisteredPlayers();

public:
	// 프리셋 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Presets")
	TMap<FName, FCameraPreset> CameraPresets; // 카메라 프리셋 모음. 이름으로 검색하여 저장된 카메라 설정을 불러올 수 있음

	// 기본 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	float MinGroupZoomDistance = 800.0f; // 그룹 모드에서 최소 줌 거리. 카메라가 이보다 더 가까이 줌인되지 않음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	float MaxGroupZoomDistance = 2000.0f; // 그룹 모드에서 최대 줌 거리. 카메라가 이보다 더 멀리 줌아웃되지 않음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	float TargetPadding = 100.0f; // 타겟 주변 여백. 화면에 모든 타겟이 표시될 때 가장자리에 추가되는 여유 공간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	float ZoomSmoothingFactor = 5.0f; // 줌 부드러움 계수. 값이 클수록 줌 전환이 더 빠르게 이루어짐

	// 등록된 플레이어 목록
	UPROPERTY(ReplicatedUsing=OnRep_RegisteredPlayers)
	TArray<ASmashCharacter*> RegisteredPlayers; // 카메라 시스템에 등록된 플레이어 목록. 네트워크로 복제됨

	// 자동 카메라 활성화 여부
	UPROPERTY(Replicated)
	bool bAutoCameraEnabled; // 자동 카메라 모드 활성화 여부. 활성화되면 상황에 따라 자동으로 카메라 모드 전환

private:
	// 현재 카메라 모드 - 복제 속성 제거 (각 클라이언트가 독립적으로 설정)
	UPROPERTY(VisibleInstanceOnly, Category = "Camera|State")
	ECameraMode CurrentCameraMode; // 현재 설정된 카메라 모드. 개인 모드, 그룹 모드, 커스텀 모드 중 하나

	// 대상 액터 목록
	UPROPERTY()
	TArray<AActor*> TargetActors; // 카메라가 추적하는 대상 액터 목록. 그룹 모드에서 모든 타겟이 화면에 표시됨

	// 메인 타겟 (기본 추적 대상)
	UPROPERTY()
	AActor* MainTarget; // 카메라의 주 추적 대상. 개인 모드에서 이 타겟을 중심으로 카메라가 위치함

	// 마스터 카메라 여부
	UPROPERTY(Replicated)
	bool bIsMasterCamera; // 이 카메라가 마스터 카메라인지 여부. 마스터 카메라는 중요한 이벤트와 뷰 전환을 관리함

	// 커스텀 카메라 액터
	UPROPERTY(Replicated)
	ACameraActor* CustomCameraActor; // 커스텀 모드에서 사용할 외부 카메라 액터. 사전 배치된 카메라 위치 활용 가능

	// 카메라 흔들림 효과 처리 변수
	bool bIsCameraShaking; // 카메라 흔들림 효과 진행 중 여부
	float ShakeIntensity; // 카메라 흔들림 강도. 값이 클수록 더 크게 흔들림
	float ShakeDuration; // 카메라 흔들림 지속 시간(초)
	float ShakeTimer; // 현재 카메라 흔들림 경과 시간(초)
	float ShakeFalloff; // 카메라 흔들림 감쇠율. 시간에 따라 흔들림이 감소하는 정도
	FVector OriginalCameraLocation; // 카메라 흔들림 시작 전 원래 위치. 효과 종료 후 복원에 사용

	// 리드 오프셋 보간
	FVector CurrentGroupPositionOffset; // 그룹 모드에서 현재 위치 오프셋. 여러 타겟의 중심 위치로 부드럽게 이동하는데 사용

	// 이전 카메라 모드를 저장하기 위한 변수
	UPROPERTY()
	ECameraMode PreviousCameraMode; // 이전 카메라 모드. 트리거 영역에서 나갈 때 복원하는데 사용됨
};
