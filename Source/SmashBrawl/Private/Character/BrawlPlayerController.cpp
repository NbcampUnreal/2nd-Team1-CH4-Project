// 


#include "SmashBrawl/Public/Character/BrawlPlayerController.h"
#include "EnhancedInputSubsystems.h"

ABrawlPlayerController::ABrawlPlayerController() :
InputMappingContext(nullptr),
BasicAttackAction(nullptr),
SpecialAttackAction(nullptr)
{
}

void ABrawlPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
