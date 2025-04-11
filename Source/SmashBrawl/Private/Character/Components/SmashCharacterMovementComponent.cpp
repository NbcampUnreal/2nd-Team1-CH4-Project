// 

#include "Character/Components/SmashCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USmashCharacterMovementComponent::USmashCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 기본값 설정
	bCanFlip = true;
}

// 네트워크 복제 속성 등록
void USmashCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// bCanFlip 속성 복제 등록
	DOREPLIFETIME(USmashCharacterMovementComponent, bCanFlip);
}

// Called when the game starts
void USmashCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USmashCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USmashCharacterMovementComponent::RequestTurnAround()
{
	if (!bCanFlip)
		return false;
	
	return Super::RequestTurnAround();
}

// bCanFlip이 복제되었을 때 호출
void USmashCharacterMovementComponent::OnRep_bCanFlip()
{
	// 클라이언트에서 복제된 값이 변경되었을 때 필요한 처리
	// 예: 애니메이션 상태 업데이트 등
}