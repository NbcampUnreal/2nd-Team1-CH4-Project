// #pragma once
//
// #include "CoreMinimal.h"
// #include "CameraTrigger.h"
// #include "SmashCameraComponent.h"
// #include "SmashCameraTrigger.generated.h"
//
// UCLASS()
// class SMASHBRAWL_API ASmashCameraTrigger : public ACameraTrigger
// {
// 	GENERATED_BODY()
//
// public:
// 	// Sets default values for this actor's properties
// 	ASmashCameraTrigger();
//
// protected:
// 	virtual void BeginPlay() override;
//
// 	/** 캐릭터가 트리거 영역에 진입했을 때 호출됩니다. */
// 	virtual void OnOverlapBegin_Implementation(class AActor* ThisActor, class AActor* OtherActor) override;
//
// 	/** 캐릭터가 트리거 영역을 벗어났을 때 호출됩니다. */
// 	virtual void OnOverlapEnd_Implementation(class AActor* ThisActor, class AActor* OtherActor) override;
//
// 	/** 카메라 파라미터를 업데이트합니다. */
// 	virtual void InitiateCameraUpdate_Implementation(AActor* Actor, bool Undo) override;
//
// public:
// 	/** 
// 	 * 블렌드 시간 오버라이드
// 	 * 이 값이 0보다 크면 CameraPreset의 BlendTime 대신 이 값을 사용합니다.
// 	 */
// 	UPROPERTY(EditAnywhere, Category = "Camera|Preset")
// 	float CustomBlendTime = 0.0f;
//
// private:
// 	/** 캐시된 SmashCameraComponent 참조 */
// 	UPROPERTY()
// 	TObjectPtr<USmashCameraComponent> SmashCameraComp;
//
// 	/** SmashCameraComponent를 찾아서 캐싱합니다. */
// 	USmashCameraComponent* GetSmashCameraComponent(AActor* Actor);
// };