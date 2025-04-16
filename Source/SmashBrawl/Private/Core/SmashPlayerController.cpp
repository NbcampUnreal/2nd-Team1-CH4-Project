// 


#include "Core/SmashPlayerController.h"

#include "Blueprint/UserWidget.h"

ASmashPlayerController::ASmashPlayerController()
{
	WidgetPtr = nullptr;
}

void ASmashPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		WidgetPtr = CreateWidget<UUserWidget>(this, WidgetClass);
		if (WidgetPtr)
		{
			WidgetPtr->AddToViewport();
		}
		
	}
}
