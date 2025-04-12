#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CameraTriggerUpdateContents.h"
#include "FollowCameraComponent.generated.h"


/**
 * 자동으로 부착된 캐릭터를 따라가는 카메라 컴포넌트입니다.
 * CameraTrigger와 함께 작동하도록 설계되어, 소유 캐릭터가 특정 영역에  진입할 때 카메라 설정을 조정할 수 있습니다.
 * 
 * 확장: Player 모드와 Group 모드를 지원합니다.
 * - Player 모드: 기존처럼 단일 플레이어에 초점
 * - Group 모드: 여러 액터(최대 4명의 플레이어 + 보스)를 화면에 표시
 */
UCLASS()
class SST_API UFollowCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	/** 생성자: 기본 값을 초기화하고 컴포넌트 설정을 구성합니다 */
	UFollowCameraComponent();

private:
	/** 컴포넌트 소유자(Character)를 캐싱하여 반복적인 조회를 방지합니다 */
	void CacheOwner();

protected:
	/** 게임 시작 시 호출되며, 초기 설정을 구성합니다 */
	virtual void BeginPlay() override;

public:
	/** 매 프레임 호출되어 카메라 위치와 회전을 업데이트합니다 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* 다음 게터들은 현재 보간 중일 경우 즉각적인 값과 다를 수 있는 목표값을 반환합니다 */

	/** 카메라 줌 거리를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetZoomDistance() const { return CameraZoomDistance; }

	/** 최대 리드(선행) 거리를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetMaxLeadDistance() const { return CameraLeadMaxDistance; }

	/** 카메라 리드(선행) 속도를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetLeadSpeed() const { return CameraLeadSpeed; }

	/** Z축 고정 높이를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetZLockHeight() const { return CameraZLockHeight; }

	/** Z축 고정 상태를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE bool GetZLock() const { return CameraZLock; }

	/** 최대 리드 거리 고정 상태를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE bool GetFixLeadAtMax() const { return FixLeadAtMax; }

	/** 카메라 위치 오프셋을 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE FVector GetLocationOffset() const { return CameraLocationOffset; }

	/** 카메라 회전 오프셋을 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE FVector GetRotationOffset() const { return CameraRotationOffset; }

	/* 수동으로 호출하거나 블루프린트에서 다른 커브를 사용해 동작을 정의할 수 있습니다 */

	/** 카메라 줌 거리를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetZoomDistance(float Value, float BlendTime);

	/** 최대 리드 거리를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetMaxLeadDistance(float Value);

	/** 카메라 리드 속도를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetLeadSpeed(float Value);

	/** Z축 고정 높이를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetZLockHeight(float Value, float BlendTime);

	/** Z축 고정 상태를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetZLock(bool Value);

	/** 최대 리드 거리 고정 상태를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetFixLeadAtMax(bool Value);

	/** 카메라 위치 오프셋을 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetLocationOffset(FVector Value, float BlendTime);

	/** 카메라 회전 오프셋을 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetRotationOffset(FVector Value, float BlendTime);

	/** CameraTrigger 객체가 세부 내용 없이 카메라 매개변수를 업데이트할 수 있는 게터/세터 */
	/** 기본 카메라 블렌드 속도를 반환합니다 */
	FORCEINLINE float GetDefaultCameraBlendSpeed() const { return CameraBlendSpeed; }

	/** 부동 소수점 매개변수를 업데이트하거나 가져옵니다 */
	float UpdateOrGetFloatParameter(ECameraTriggerUpdateFloatParameter Parameter, FCameraTriggerUpdateFloat* Update = nullptr);

	/** 부울 매개변수를 업데이트하거나 가져옵니다 */
	bool UpdateOrGetBoolParameter(ECameraTriggerUpdateBoolParameter Parameter, FCameraTriggerUpdateBool* Update = nullptr);

	/** 벡터 매개변수를 업데이트하거나 가져옵니다 */
	FVector UpdateOrGetVectorParameter(ECameraTriggerUpdateVectorParameter Parameter, FCameraTriggerUpdateVector* Update = nullptr);

	/** 카메라 모드를 설정합니다 (Player/Group) */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraMode(ECameraMode NewMode);

	/** 현재 카메라 모드를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE ECameraMode GetCameraMode() const { return CurrentCameraMode; }

	/** 카메라 모드 매개변수를 업데이트하거나 가져옵니다 */
	ECameraMode UpdateOrGetModeParameter(ECameraTriggerUpdateModeParameter Parameter, FCameraTriggerUpdateMode* Update = nullptr);

	/** 그룹 모드에 액터를 추가합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void AddActorToGroup(AActor* ActorToAdd);

	/** 그룹 모드에서 액터를 제거합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void RemoveActorFromGroup(AActor* ActorToRemove);

	/** 그룹 모드에서 모든 액터를 제거합니다 (소유자 제외) */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void ClearGroupActors();

	/** 그룹 모드의 줌 거리를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera|Group")
	void SetGroupZoomDistance(float Value, float BlendTime);

	/** 그룹 모드의 패딩(여백)을 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera|Group")
	void SetGroupPadding(float Value);

	/** 그룹 모드의 위치 오프셋을 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera|Group")
	void SetGroupLocationOffset(FVector Value, float BlendTime);

	/** 그룹 모드의 회전 오프셋을 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera|Group")
	void SetGroupRotationOffset(FVector Value, float BlendTime);

	/** 그룹 모드의 Z축 고정 높이를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera|Group")
	void SetGroupZLockHeight(float Value, float BlendTime);

	/** 그룹 모드의 Z축 고정 상태를 설정합니다 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera|Group")
	void SetGroupZLock(bool Value);

	/** 그룹 모드의 줌 거리를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	FORCEINLINE float GetGroupZoomDistance() const { return GroupZoomDistance; }

	/** 그룹 모드의 패딩(여백)을 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	FORCEINLINE float GetGroupPadding() const { return GroupPadding; }

	/** 그룹 모드의 위치 오프셋을 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	FORCEINLINE FVector GetGroupLocationOffset() const { return GroupLocationOffset; }

	/** 그룹 모드의 회전 오프셋을 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	FORCEINLINE FVector GetGroupRotationOffset() const { return GroupRotationOffset; }

	/** 그룹 모드의 Z축 고정 높이를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	FORCEINLINE float GetGroupZLockHeight() const { return GroupZLockHeight; }

	/** 그룹 모드의 Z축 고정 상태를 반환합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	FORCEINLINE bool GetGroupZLock() const { return GroupZLock; }

	/** 보스 액터를 반환합니다 */
	FORCEINLINE AActor* GetBossActor() const { return BossActor; }

	/** 그룹 액터 목록을 반환합니다 */
	FORCEINLINE const TArray<AActor*>& GetGroupActors() const { return GroupActors; }

	/** 추가 추적 액터 목록을 반환합니다 */
	FORCEINLINE const TArray<AActor*>& GetAdditionalActorsToTrack() const { return AdditionalActorsToTrack; }

public:
	/** 게임 모드와의 호환성을 위한 메서드: 추적할 액터를 그룹에 추가합니다 */
	void AddActorToTrack(AActor* ActorToAdd);

	/** 게임 모드와의 호환성을 위한 메서드: 추적 중인 액터를 그룹에서 제거합니다 */
	void RemoveActorToTrack(AActor* ActorToRemove);

	/** 게임 모드와의 호환성을 위한 메서드: 보스 액터를 설정합니다 */
	void SetBossActor(AActor* NewBossActor);

	/** 자동 줌 기능을 켜거나 끕니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void SetUseAutomaticZoom(bool bEnable);

	/** 자동 위치 조정 기능을 켜거나 끕니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void SetUseAutomaticOffset(bool bEnable);

	/** 보스 가중치를 설정합니다 (보스가 카메라 위치에 미치는 영향력) */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void SetBossWeightFactor(float NewFactor);

	/** 자동 줌 계수를 설정합니다 */
	UFUNCTION(BlueprintCallable, Category = "Camera|Group")
	void SetAutoZoomFactor(float NewFactor);

public:
	/** 카메라 줌 거리: 카메라가 캐릭터로부터 떨어진 거리를 제어합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	float CameraZoomDistance;

	/** 캐릭터 리드(선행) 최대 거리: 팔로우 모드에서 카메라가 캐릭터를 선행하는 최대 거리입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	float CameraLeadMaxDistance;

	/** 카메라 리드 속도: 팔로우 모드에서 카메라가 캐릭터를 선행하는 속도를 조절합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	float CameraLeadSpeed;

	/** 리드 고정: true일 경우 카메라가 움직임에 관계없이 최대 리드 거리(및 오프셋)에 고정됩니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	bool FixLeadAtMax;

	/** Z축 고정: true일 경우 카메라가 캐릭터의 점프를 따라가지 않고 특정 z-높이에 고정됩니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	bool CameraZLock;

	/** Z축 고정 높이: CameraZLock이 true일 때 카메라의 Z 높이 값입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	float CameraZLockHeight;

	/** 카메라 블렌드 속도: 레벨의 다른 카메라로 전환할 때 블렌드 속도를 제어합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	float CameraBlendSpeed;

	/** 위치 오프셋: 카메라 트랜스폼에 추가할 정적 오프셋입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	FVector CameraLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 회전 오프셋: 카메라에 추가할 정적 회전값입니다 (롤, 피치, 요) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	FVector CameraRotationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 리드 잠금: true일 경우 카메라가 리드(선행) 위치를 고정합니다 (대시 등의 상황에서 사용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	bool CameraLeadLock;

	/** 웅크림 높이 감소: 웅크릴 때 카메라 이동을 방지하기 위한 캡슐 반높이 차이 값입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	float CrouchHeightReduction;

	/** 현재 카메라 모드: Player 또는 Group 모드를 나타냅니다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Camera|Config|Player", meta = (AllowPrivateAccess = "true"))
	ECameraMode CurrentCameraMode = ECameraMode::Player;

	/** 그룹 모드에서 추적할 액터 목록: 그룹 모드에서 카메라가 추적하는 모든 액터 목록입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> GroupActors;

	/** 그룹 모드 줌 거리: 그룹 모드에서 카메라의 기본 줌 거리입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float GroupZoomDistance = 700.0f;

	/** 그룹 패딩: 그룹 모드에서 모든 액터를 화면에 표시하기 위한 추가 여백입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float GroupPadding = 150.0f;

	/** 그룹 위치 오프셋: 그룹 모드에서 카메라 트랜스폼에 추가할 정적 오프셋입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	FVector GroupLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 그룹 회전 오프셋: 그룹 모드에서 카메라에 추가할 정적 회전값입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	FVector GroupRotationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 그룹 Z축 고정: 그룹 모드에서 카메라의 Z축 고정 상태를 제어합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	bool GroupZLock = true;

	/** 그룹 Z축 고정 높이: 그룹 모드에서 Z축 고정시 카메라의 높이 값입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float GroupZLockHeight = 500.0f;

	/** 자동 줌 사용 여부: 그룹 내 액터 분포에 따라 자동으로 줌을 조정합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	bool bUseAutomaticZoom = true;

	/** 자동 위치 조정 사용 여부: 그룹 내 액터 분포에 따라 자동으로 위치를 조정합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	bool bUseAutomaticOffset = true;

	/** 최소 그룹 줌 거리: 자동 줌 계산 시 적용되는 최소 줌 거리입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float MinGroupZoomDistance = 600.0f;

	/** 최대 그룹 줌 거리: 자동 줌 계산 시 적용되는 최대 줌 거리입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float MaxGroupZoomDistance = 1800.0f;

	/** 자동 줌 계수: 액터 간 거리에 따른 줌 배율입니다 (높을수록 더 많이 줌아웃됩니다) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float AutoZoomFactor = 3.0f;

	/** 보스 가중치 계수: 보스의 위치가 카메라 위치 계산에 미치는 영향력입니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Config|Group", meta = (AllowPrivateAccess = "true"))
	float BossWeightFactor = 1.5f;

	/** 디버그 모드: 활성화 시 카메라 계산 관련 디버그 로그를 출력합니다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smash Camera|Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebugMode = false;

private:
	/** 보스 액터: 그룹 모드에서 특별 취급되는 보스 액터입니다 */
	UPROPERTY()
	AActor* BossActor;

	/** 추가 추적 액터 목록: 그룹 모드에서 추가적으로 추적하는 액터 목록입니다 */
	UPROPERTY()
	TArray<AActor*> AdditionalActorsToTrack;

	/** 소유자: 카메라가 부착된 캐릭터입니다 */
	UPROPERTY()
	class ACharacter* Owner;

	/** 현재 X축 리드 오프셋: 캐릭터 전방에 대한 현재 리드 오프셋 값입니다 */
	float CurrentLeadOffsetX = 0.0f;

	/** 현재 줌 거리: 현재 보간 중인 줌 거리 값입니다 */
	float CurrentZoomDistance = 0.0f;

	/** 줌 거리 블렌드 속도: 새로운 줌 거리로 보간하는 속도입니다 */
	float ZoomDistanceBlendSpeed = 1.0f;

	/** 현재 Z축 고정 높이: 현재 보간 중인 Z축 고정 높이 값입니다 */
	float CurrentZLockHeight = 0.0f;

	/** Z축 고정 높이 블렌드 속도: 새로운 Z축 고정 높이로 보간하는 속도입니다 */
	float ZLockHeightBlendSpeed = 1.0f;

	/** 현재 위치 오프셋: 현재 보간 중인 위치 오프셋입니다 */
	FVector CurrentLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 위치 오프셋 블렌드 속도: 새로운 위치 오프셋으로 보간하는 속도입니다 */
	float LocationOffsetBlendSpeed = 1.0f;

	/** 현재 회전 오프셋: 현재 보간 중인 회전 오프셋입니다 */
	FVector CurrentRotationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 회전 오프셋 블렌드 속도: 새로운 회전 오프셋으로 보간하는 속도입니다 */
	float RotationOffsetBlendSpeed = 1.0f;

	/** 그룹 줌 거리 블렌드 속도: 그룹 모드에서 새로운 줌 거리로 보간하는 속도입니다 */
	float GroupZoomDistanceBlendSpeed = 1.0f;

	/** 그룹 Z축 고정 높이 블렌드 속도: 그룹 모드에서 새로운 Z축 고정 높이로 보간하는 속도입니다 */
	float GroupZLockHeightBlendSpeed = 1.0f;

	/** 그룹 위치 오프셋 블렌드 속도: 그룹 모드에서 새로운 위치 오프셋으로 보간하는 속도입니다 */
	float GroupLocationOffsetBlendSpeed = 1.0f;

	/** 그룹 회전 오프셋 블렌드 속도: 그룹 모드에서 새로운 회전 오프셋으로 보간하는 속도입니다 */
	float GroupRotationOffsetBlendSpeed = 1.0f;

	/** 현재 그룹 줌 거리: 그룹 모드에서 현재 보간 중인 줌 거리 값입니다 */
	float CurrentGroupZoomDistance = 0.0f;

	/** 현재 그룹 Z축 고정 높이: 그룹 모드에서 현재 보간 중인 Z축 고정 높이 값입니다 */
	float CurrentGroupZLockHeight = 0.0f;

	/** 현재 그룹 위치 오프셋: 그룹 모드에서 현재 보간 중인 위치 오프셋입니다 */
	FVector CurrentGroupLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 현재 그룹 회전 오프셋: 그룹 모드에서 현재 보간 중인 회전 오프셋입니다 */
	FVector CurrentGroupRotationOffset = FVector(0.0f, 0.0f, 0.0f);

	/** 그룹 카메라 위치 계산: 그룹 모드에서 매 프레임 카메라 위치를 계산합니다 */
	void CalculateGroupCameraPosition(float DeltaTime);

	/** 그룹 중심 계산: 모든 추적 액터의 중심 위치를 계산합니다 */
	FVector CalculateGroupCenter() const;

	/** 필요한 오프셋 계산: 그룹 모드에서 모든 액터를 화면에 표시하기 위한 오프셋을 계산합니다 */
	FVector CalculateRequiredOffset(const FVector& GroupCenter) const;

	/** 필요한 줌 계산: 그룹 모드에서 모든 액터를 화면에 표시하기 위한 줌 거리를 계산합니다 */
	float CalculateRequiredZoom() const;

	/** 모든 액터의 3D 바운딩 박스를 계산합니다 */
	void CalculateGroupBounds(FVector& OutMin, FVector& OutMax) const;

	/** 적절한 그룹 카메라 위치를 계산합니다 (3D 공간 고려) */
	FVector CalculateOptimalCameraPosition(const FVector& GroupCenter, float ZoomDistance, float InCurrentZLockHeight) const;
};
