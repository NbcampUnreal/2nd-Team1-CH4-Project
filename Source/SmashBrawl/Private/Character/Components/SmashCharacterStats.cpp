#include "Character/Components/SmashCharacterStats.h"

#include "Core/SmashGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/HUD/InGame/UW_HUD_CharacterInfo.h"

// Sets default values for this component's properties
USmashCharacterStats::USmashCharacterStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// ...
}


void USmashCharacterStats::BeginPlay()
{
	Super::BeginPlay();

	USmashGameInstance* SmashGameInstance = Cast<USmashGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!SmashGameInstance)
		return;

	Parent = Cast<ASmashCharacter>(GetOwner());
	if (!Parent)
		return;

	if (SmashGameInstance->SubMode == ESmashInGameMode::Stock)
	{
		Stock = SmashGameInstance->StockCount;
	}
}

void USmashCharacterStats::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USmashCharacterStats, HP);
	DOREPLIFETIME(USmashCharacterStats, Percent);
	DOREPLIFETIME(USmashCharacterStats, HPMAX);
	DOREPLIFETIME(USmashCharacterStats, Stock);
}


// Called every frame
void USmashCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USmashCharacterStats::TakeDamage(int32 NewDamage)
{
	if (Parent->HitStates == ESmashHitState::Normal)
	{
		Percent = NewDamage + Percent;
		SuperIndex = (NewDamage * 0.5f) + SuperIndex;

		Parent->UW_HUDCharacterInfo->PlayShake();
	}
}

void USmashCharacterStats::Screenshake(float RumbleInten, float RumbleDuration)
{
	UGameplayStatics::PlayWorldCameraShake(this, ShakeClass, Parent->GetActorLocation(), RumbleInten, RumbleDuration);
}
