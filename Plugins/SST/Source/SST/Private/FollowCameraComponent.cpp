#include "FollowCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UFollowCameraComponent::UFollowCameraComponent(): CameraZoomDistance(0), CameraLeadMaxDistance(0), CameraLeadSpeed(0), FixLeadAtMax(false), CameraZLock(false), CameraZLockHeight(0), CameraBlendSpeed(0), CameraLeadLock(false),
                                                  CrouchHeightReduction(0), Owner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFollowCameraComponent::CacheOwner()
{
	if (Owner)
	{
		return;
	}

	Owner = Cast<ACharacter>(GetOwner());
	check(Owner != nullptr);
}

void UFollowCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheOwner();

	CurrentZoomDistance = CameraZoomDistance;
	CurrentZLockHeight = CameraZLockHeight;
	CurrentLocationOffset = CameraLocationOffset;
	CurrentRotationOffset = CameraRotationOffset;
}

float UFollowCameraComponent::UpdateOrGetFloatParameter(ECameraTriggerUpdateFloatParameter Parameter, FCameraTriggerUpdateFloat* Update)
{
	if (!Owner)
	{
		CacheOwner();
	}

	float ret = 0.0f;
	switch (Parameter)
	{
	case ECameraTriggerUpdateFloatParameter::ZoomDistance:
		if (Update)
		{
			SetZoomDistance(Update->Value, Update->BlendTime);
		}
		ret = CameraZoomDistance;
		break;
	case ECameraTriggerUpdateFloatParameter::MaxLeadDistance:
		if (Update)
		{
			SetMaxLeadDistance(Update->Value);
		}
		ret = CameraLeadMaxDistance;
		break;
	case ECameraTriggerUpdateFloatParameter::LeadSpeed:
		if (Update)
		{
			SetLeadSpeed(Update->Value);
		}
		ret = CameraLeadSpeed;
		break;
	case ECameraTriggerUpdateFloatParameter::ZLockHeight:
		if (Update)
		{
			SetZLockHeight(Update->Value, Update->BlendTime);
		}
		ret = CameraZLockHeight;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown float parameter: %d"), Parameter);
		break;
	}
	return ret;
}

bool UFollowCameraComponent::UpdateOrGetBoolParameter(ECameraTriggerUpdateBoolParameter Parameter, FCameraTriggerUpdateBool* Update)
{
	if (!Owner) { CacheOwner(); }

	bool ret = false;
	switch (Parameter)
	{
	case ECameraTriggerUpdateBoolParameter::ZLock:
		if (Update)
		{
			SetZLock(Update->Value);
		}
		ret = CameraZLock;
		break;
	case ECameraTriggerUpdateBoolParameter::FixLeadAtMax:
		if (Update)
		{
			SetFixLeadAtMax(Update->Value);
		}
		ret = FixLeadAtMax;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown bool parameter: %d"), Parameter);
		break;
	}
	return ret;
}

FVector UFollowCameraComponent::UpdateOrGetVectorParameter(ECameraTriggerUpdateVectorParameter Parameter, FCameraTriggerUpdateVector* Update)
{
	if (!Owner) { CacheOwner(); }

	FVector ret{};
	switch (Parameter)
	{
	case ECameraTriggerUpdateVectorParameter::LocationOffset:
		if (Update)
		{
			SetLocationOffset(Update->Value, Update->BlendTime);
		}
		ret = CameraLocationOffset;
		break;
	case ECameraTriggerUpdateVectorParameter::RotationOffset:
		if (Update)
		{
			SetRotationOffset(Update->Value, Update->BlendTime);
		}
		ret = CameraRotationOffset;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Camera update received unknown vector parameter: %d"), Parameter);
		break;
	}
	return ret;
}

void UFollowCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner) { CacheOwner(); }

    // 카메라 리드 오프셋 계산
    if (!CameraLeadLock)
	{
        float target = FixLeadAtMax ? 
            Owner->GetActorForwardVector().X : 
            Owner->GetVelocity().X / Owner->GetCharacterMovement()->GetMaxSpeed();
		target *= CameraLeadMaxDistance;
		CurrentLeadOffsetX = FMath::FInterpTo(CurrentLeadOffsetX, target, DeltaTime, CameraLeadSpeed);
	}

    // 보간 값들 업데이트
	CurrentZoomDistance = FMath::FInterpTo(CurrentZoomDistance, CameraZoomDistance, DeltaTime, ZoomDistanceBlendSpeed);
	CurrentZLockHeight = FMath::FInterpTo(CurrentZLockHeight, CameraZLockHeight, DeltaTime, ZLockHeightBlendSpeed);
	CurrentLocationOffset = FMath::VInterpTo(CurrentLocationOffset, CameraLocationOffset, DeltaTime, LocationOffsetBlendSpeed);
	CurrentRotationOffset = FMath::VInterpTo(CurrentRotationOffset, CameraRotationOffset, DeltaTime, RotationOffsetBlendSpeed);

    // 카메라 위치 계산
	FVector targetLocation = Owner->GetActorLocation();
	
    // X축: 리드 오프셋 + 위치 오프셋
	targetLocation.X += CurrentLeadOffsetX + CurrentLocationOffset.X;
    
    // Y축: 줌 거리 + 위치 오프셋
	targetLocation.Y += CurrentZoomDistance + CurrentLocationOffset.Y;

    // 웅크림 보정
	float crouchOffset = Owner->bIsCrouched ? CrouchHeightReduction : 0.0f;

    // Z축: Z고정 사용 시 고정 높이, 아니면 캐릭터 높이 + 오프셋
    targetLocation.Z = CameraZLock ? 
        CurrentZLockHeight : 
        (targetLocation.Z + CurrentLocationOffset.Z + crouchOffset);

    // 계산된 위치로 카메라 이동
	SetWorldLocation(targetLocation);

    // 카메라 회전 계산 및 적용
	FVector adjustedRotation = FVector(CurrentRotationOffset.Y, CurrentRotationOffset.X, CurrentRotationOffset.Z);
    adjustedRotation.Z -= 90.0f; // 사이드뷰 설정을 위한 -90도 회전
	SetWorldRotation(FQuat::MakeFromEuler(adjustedRotation));
}


void UFollowCameraComponent::SetZoomDistance_Implementation(float Value, float BlendTime)
{
	CameraZoomDistance = Value;
	ZoomDistanceBlendSpeed = 1 / BlendTime;
}

void UFollowCameraComponent::SetMaxLeadDistance_Implementation(float Value)
{
	CameraLeadMaxDistance = Value;
}

void UFollowCameraComponent::SetLeadSpeed_Implementation(float Value)
{
	CameraLeadSpeed = Value;
}

void UFollowCameraComponent::SetZLockHeight_Implementation(float Value, float BlendTime)
{
	CameraZLockHeight = Value;
	ZLockHeightBlendSpeed = 1 / BlendTime;
}

void UFollowCameraComponent::SetZLock_Implementation(bool Value)
{
	CameraZLock = Value;

	// Ensure that we don't snap to a new position when turning on/off lock. 
	CurrentLocationOffset.Z = GetComponentLocation().Z - Owner->GetActorLocation().Z;
	CurrentZLockHeight = GetComponentLocation().Z;
}

void UFollowCameraComponent::SetFixLeadAtMax_Implementation(bool Value)
{
	FixLeadAtMax = Value;
}

void UFollowCameraComponent::SetLocationOffset_Implementation(FVector Value, float BlendTime)
{
	CameraLocationOffset = Value;
	LocationOffsetBlendSpeed = 1 / BlendTime;
}

void UFollowCameraComponent::SetRotationOffset_Implementation(FVector Value, float BlendTime)
{
	CameraRotationOffset = Value;
	RotationOffsetBlendSpeed = 1 / BlendTime;
}
