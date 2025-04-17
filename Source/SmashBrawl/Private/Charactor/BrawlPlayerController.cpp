#include "Charactor/BrawlPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SmashGameInstance.h"

void ABrawlPlayerController::BeginPlay()
{
	Super::BeginPlay();

	USmashGameInstance* GI = Cast<USmashGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->bShouldShowHostSuccess)
	{
		GI->bShouldShowHostSuccess = false; // 다시 false로 리셋
		GI->ShowWidget(GI->HostSuccessWidgetClass); // 여기서 생성
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && OnlineTestWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(PC, OnlineTestWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
}
