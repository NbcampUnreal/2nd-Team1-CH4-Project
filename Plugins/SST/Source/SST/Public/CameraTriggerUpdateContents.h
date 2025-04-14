// Copyright 2024 NFB Makes Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraTriggerUpdateContents.generated.h"

// 카메라 모드 열거형 추가
UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	Player UMETA(DisplayName = "Player"),
	Group UMETA(DisplayName = "Group")
};

UENUM(BlueprintType)
enum class ECameraTriggerUpdateFloatParameter : uint8
{
	ZoomDistance UMETA(DisplayName = "ZoomDistance"),
	MaxLeadDistance   UMETA(DisplayName = "MaxLeadDistance"),
	LeadSpeed      UMETA(DisplayName = "LeadSpeed"),
	ZLockHeight   UMETA(DisplayName = "ZLockHeight")
};

USTRUCT(BlueprintType)
struct SST_API FCameraTriggerUpdateFloat
{
	GENERATED_BODY()

	/** Which parameter to update */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ECameraTriggerUpdateFloatParameter Parameter = ECameraTriggerUpdateFloatParameter::ZoomDistance;

	/** New value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float Value = 0.0f;

	/** Total time over which to make a smooth transition. Instant if set to 0 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BlendTime = 0.0f;
};

UENUM(BlueprintType)
enum class ECameraTriggerUpdateBoolParameter : uint8
{
	ZLock UMETA(DisplayName = "ZLock"),
	FixLeadAtMax UMETA(DisplayName = "FixLeadAtMax")
};

USTRUCT(BlueprintType)
struct SST_API FCameraTriggerUpdateBool
{
	GENERATED_BODY()

	/** Which parameter to update */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ECameraTriggerUpdateBoolParameter Parameter = ECameraTriggerUpdateBoolParameter::ZLock;

	/** New value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool Value = false;

	/** Total time over which to make a smooth transition. Instant if set to 0 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BlendTime = 0.0f;
};

UENUM(BlueprintType)
enum class ECameraTriggerUpdateVectorParameter : uint8
{
	LocationOffset UMETA(DisplayName = "LocationOffset"),
	RotationOffset UMETA(DisplayName = "RotationOffset")
};

USTRUCT(BlueprintType)
struct SST_API FCameraTriggerUpdateVector
{
	GENERATED_BODY()

	/** Which parameter to update */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ECameraTriggerUpdateVectorParameter Parameter = ECameraTriggerUpdateVectorParameter::LocationOffset;

	/** New value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector Value = FVector(0, 0, 0);

	/** Total time over which to make a smooth transition. Instant if set to 0 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BlendTime = 0.0f;
};

// 카메라 모드 업데이트 열거형 추가
UENUM(BlueprintType)
enum class ECameraTriggerUpdateModeParameter : uint8
{
    CameraMode UMETA(DisplayName = "CameraMode")
};

// 카메라 모드 업데이트 구조체 추가
USTRUCT(BlueprintType)
struct SST_API FCameraTriggerUpdateMode
{
    GENERATED_BODY()

    /** Which parameter to update */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    ECameraTriggerUpdateModeParameter Parameter = ECameraTriggerUpdateModeParameter::CameraMode;

    /** New value */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    ECameraMode Value = ECameraMode::Player;

    /** Total time over which to make a smooth transition. Instant if set to 0 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float BlendTime = 0.5f;
};