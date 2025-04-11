// Copyright 2024 NFB Makes Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CameraTriggerUpdateContents.h"
#include "FollowCameraComponent.generated.h"

/**
 * 카메라가 부착된 대상을 자동으로 따라가는 카메라 컴포넌트입니다.
 * CameraTrigger와 함께 작동하도록 설계되어, 소유 캐릭터가 특정 볼륨(영역)에 
 * 진입할 때 카메라 설정을 조정할 수 있습니다.
 */
UCLASS()
class SST_API UFollowCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UFollowCameraComponent();

private:
	/**
	 * 소유자 캐싱 함수
	 * 매 번 소유자를 가져오는 대신 한 번 캐싱하여 성능을 향상시킵니다.
	 */
	void CacheOwner();

protected:
	/**
	 * 게임 시작 시 초기화 작업을 수행합니다.
	 * 소유자를 캐싱하고 초기 카메라 값들을 설정합니다.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * 매 프레임마다 호출되어 카메라 위치와 회전을 업데이트합니다.
	 * 캐릭터의 속도에 따라 리드 오프셋을 계산하고, 보간을 통해 부드러운 카메라 이동을 구현합니다.
	 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* 다음 getter 함수들은 현재 목표 값을 반환합니다. 현재 보간 중이라면 즉각적인 값과 다를 수 있습니다 */
	/** 카메라 줌 거리 값을 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetZoomDistance() const { return CameraZoomDistance; }

	/** 카메라 최대 리드 거리 값을 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetMaxLeadDistance() const { return CameraLeadMaxDistance; }

	/** 카메라 리드 속도 값을 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetLeadSpeed() const { return CameraLeadSpeed; }

	/** 카메라 Z축 고정 높이 값을 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE float GetZLockHeight() const { return CameraZLockHeight; }

	/** 카메라 Z축 고정 여부를 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE bool GetZLock() const { return CameraZLock; }

	/** 최대 리드 거리 고정 여부를 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE bool GetFixLeadAtMax() const { return FixLeadAtMax; }

	/** 카메라 위치 오프셋을 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE FVector GetLocationOffset() const { return CameraLocationOffset; }

	/** 카메라 회전 오프셋을 가져옵니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FORCEINLINE FVector GetRotationOffset() const { return CameraRotationOffset; }

	/* 다음 함수들은 수동으로 호출하거나 블루프린트에서 오버라이드하여 다양한 커브를 사용한 동작을 정의할 수 있습니다 */
	/**
	 * 카메라 줌 거리를 설정합니다.
	 * @param Value - 설정할 줌 거리 값
	 * @param BlendTime - 현재 값에서 새 값으로 전환할 시간(초)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetZoomDistance(float Value, float BlendTime);

	/**
	 * 카메라 최대 리드 거리를 설정합니다.
	 * @param Value - 설정할 최대 리드 거리 값
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetMaxLeadDistance(float Value);

	/**
	 * 카메라 리드 속도를 설정합니다.
	 * @param Value - 설정할 리드 속도 값
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetLeadSpeed(float Value);

	/**
	 * 카메라 Z축 고정 높이를 설정합니다.
	 * @param Value - 설정할 Z축 높이 값
	 * @param BlendTime - 현재 값에서 새 값으로 전환할 시간(초)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetZLockHeight(float Value, float BlendTime);

	/**
	 * 카메라 Z축 고정 여부를 설정합니다.
	 * @param Value - Z축 고정 여부(true/false)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetZLock(bool Value);

	/**
	 * 최대 리드 거리 고정 여부를 설정합니다.
	 * @param Value - 고정 여부(true/false)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetFixLeadAtMax(bool Value);

	/**
	 * 카메라 위치 오프셋을 설정합니다.
	 * @param Value - 설정할 위치 오프셋 벡터
	 * @param BlendTime - 현재 값에서 새 값으로 전환할 시간(초)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetLocationOffset(FVector Value, float BlendTime);

	/**
	 * 카메라 회전 오프셋을 설정합니다.
	 * @param Value - 설정할 회전 오프셋 벡터
	 * @param BlendTime - 현재 값에서 새 값으로 전환할 시간(초)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void SetRotationOffset(FVector Value, float BlendTime);

	// CameraTrigger가 카메라 매개변수에 대해 자세히 알 필요 없이 업데이트할 수 있게 해주는 Setter/Getter 함수들
	/** 기본 카메라 블렌드 속도를 반환합니다. */
	FORCEINLINE float GetDefaultCameraBlendSpeed() const { return CameraBlendSpeed; }

	/**
	 * 카메라 float 매개변수를 업데이트하거나 가져옵니다.
	 * @param Parameter - 업데이트할 매개변수 유형
	 * @param Update - 업데이트 정보(없으면 현재 값만 반환)
	 * @return 현재 또는 업데이트된 값
	 */
	float UpdateOrGetFloatParameter(ECameraTriggerUpdateFloatParameter Parameter, FCameraTriggerUpdateFloat* Update = nullptr);

	/**
	 * 카메라 bool 매개변수를 업데이트하거나 가져옵니다.
	 * @param Parameter - 업데이트할 매개변수 유형
	 * @param Update - 업데이트 정보(없으면 현재 값만 반환)
	 * @return 현재 또는 업데이트된 값
	 */
	bool UpdateOrGetBoolParameter(ECameraTriggerUpdateBoolParameter Parameter, FCameraTriggerUpdateBool* Update = nullptr);

	/**
	 * 카메라 벡터 매개변수를 업데이트하거나 가져옵니다.
	 * @param Parameter - 업데이트할 매개변수 유형
	 * @param Update - 업데이트 정보(없으면 현재 값만 반환)
	 * @return 현재 또는 업데이트된 값
	 */
	FVector UpdateOrGetVectorParameter(ECameraTriggerUpdateVectorParameter Parameter, FCameraTriggerUpdateVector* Update = nullptr);

public:
	/** 
	 * 카메라 줌 거리
	 * Y축 방향으로 카메라가 캐릭터로부터 얼마나 떨어져 있을지 결정합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraZoomDistance;

	/**
	 * 카메라가 캐릭터를 선행하는 최대 거리
	 * 캐릭터 이동 시 카메라가 최대 얼마나 앞서 이동할지 결정합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraLeadMaxDistance;

	/**
	 * 카메라 선행 이동 속도 조정값
	 * 카메라가 캐릭터 이동을 얼마나 빠르게 따라잡을지 결정합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraLeadSpeed;

	/**
	 * 최대 리드 거리 고정 여부
	 * true인 경우, 이동과 관계없이 카메라가 항상 최대 리드 거리(+ 오프셋)에 고정됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool FixLeadAtMax;

	/**
	 * 카메라 Z축 고정 여부
	 * true인 경우, 캐릭터가 점프해도 카메라는 지정된 Z 높이에 고정됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool CameraZLock;

	/**
	 * 카메라 Z축 고정 높이
	 * CameraZLock이 true일 때 카메라의 Z 위치를 결정합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraZLockHeight;

	/**
	 * 카메라 블렌드 속도
	 * 레벨 내 다른 카메라로 전환할 때 블렌드 속도를 조절합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraBlendSpeed;

	/**
	 * 카메라 위치 오프셋
	 * 카메라 기본 위치에 추가할 고정 오프셋 값입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector CameraLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	/**
	 * 카메라 회전 오프셋
	 * 카메라 기본 회전에 추가할 고정 회전 값입니다(롤, 피치, 요).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector CameraRotationOffset = FVector(0.0f, 0.0f, 0.0f);

	/**
	 * 카메라 선행 고정 여부
	 * true인 경우, 카메라의 선행 위치가 고정됩니다(예: 대시 중).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool CameraLeadLock;

	/**
	 * 웅크림 시 높이 감소값
	 * 캐릭터가 웅크릴 때 카메라가 움직이지 않도록 캡슐 반높이 차이에 따라 설정합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchHeightReduction;

private:
	/**
	 * 현재 X축 리드 오프셋
	 * 캐릭터 이동 시 계산되어 카메라 위치에 적용됩니다.
	 */
	float CurrentLeadOffsetX = 0.0f;

	/**
	 * 현재 줌 거리
	 * 목표 줌 거리로 보간되는 현재 줌 값입니다.
	 */
	float CurrentZoomDistance = 0.0f;

	/**
	 * 줌 거리 보간 속도
	 * 카메라 줌이 변경될 때 부드러운 전환을 위한 보간 속도입니다.
	 */
	float ZoomDistanceBlendSpeed = 1.0f;

	/**
	 * 현재 Z축 고정 높이
	 * 목표 Z 고정 높이로 보간되는 현재 값입니다.
	 */
	float CurrentZLockHeight = 0.0f;

	/**
	 * Z축 고정 높이 보간 속도
	 * Z 고정 높이가 변경될 때 부드러운 전환을 위한 보간 속도입니다.
	 */
	float ZLockHeightBlendSpeed = 1.0f;

	/**
	 * 현재 위치 오프셋
	 * 목표 위치 오프셋으로 보간되는 현재 값입니다.
	 */
	FVector CurrentLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	/**
	 * 위치 오프셋 보간 속도
	 * 위치 오프셋이 변경될 때 부드러운 전환을 위한 보간 속도입니다.
	 */
	float LocationOffsetBlendSpeed = 1.0f;

	/**
	 * 현재 회전 오프셋
	 * 목표 회전 오프셋으로 보간되는 현재 값입니다.
	 */
	FVector CurrentRotationOffset = FVector(0.0f, 0.0f, 0.0f);

	/**
	 * 회전 오프셋 보간 속도
	 * 회전 오프셋이 변경될 때 부드러운 전환을 위한 보간 속도입니다.
	 */
	float RotationOffsetBlendSpeed = 1.0f;

private:
	/**
	 * 카메라 소유자 캐싱
	 * 틱 마다 반복 참조를 피하기 위해 소유자를 캐싱합니다.
	 */
	UPROPERTY()
	class ACharacter* Owner;
};
