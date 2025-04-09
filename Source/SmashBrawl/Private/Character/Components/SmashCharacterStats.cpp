#include "Character/Components/SmashCharacterStats.h"

#include "Character/Components/SmashCombatComponent.h"
#include "Core/SmashGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/HUD/InGame/UW_HUD_CharacterInfo.h"

// Sets default values for this component's properties
USmashCharacterStats::USmashCharacterStats()
{
	// 필요한 경우에만 Tick을 활성화
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

	// 기본값 초기화
	Percent = 0;
	Stock = 3;   // 기본 스톡 수
	DefaultGravityScale = 1.0f;
	Weight = 100.0f;
	SuperIndex = 0;
	Elims = 0;
	DamageDone = 0;
	DamageTaken = 0;
	SD = 0;
	Falls = 0;
}

void USmashCharacterStats::BeginPlay()
{
	Super::BeginPlay();

	// Parent 변수 초기화 (중복 제거)
	Parent = Cast<ASmashCharacter>(GetOwner());
	if (!Parent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SmashCharacterStats: Owner is not a SmashCharacter!"));
		return;
	}

	// 게임 인스턴스에서 스톡 설정 가져오기
	USmashGameInstance* SmashGameInstance = Cast<USmashGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (SmashGameInstance && SmashGameInstance->SubMode == ESmashInGameMode::Stock)
	{
		Stock = SmashGameInstance->StockCount;
	}
}

void USmashCharacterStats::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 실제로 사용되는 변수만 복제
	DOREPLIFETIME(USmashCharacterStats, Percent);
	DOREPLIFETIME(USmashCharacterStats, Stock);
}

void USmashCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Tick이 비활성화되어 이 함수는 호출되지 않지만, 오버라이드는 유지
}

void USmashCharacterStats::Screenshake(float RumbleInten, float RumbleDuration)
{
	if (!Parent || !ShakeClass)
	{
		return;
	}

	UGameplayStatics::PlayWorldCameraShake(this, ShakeClass, Parent->GetActorLocation(), RumbleInten, RumbleDuration);
}

void USmashCharacterStats::OnPercentChanged(int32 OldPercent, int32 NewPercent)
{
	// 내부 Percent 값 동기화
	Percent = NewPercent;

	// UI 업데이트 등 추가 작업 (null 체크 추가)
	if (Parent && Parent->UW_HUDCharacterInfo)
	{
		Parent->UW_HUDCharacterInfo->PlayShake();
	}
}