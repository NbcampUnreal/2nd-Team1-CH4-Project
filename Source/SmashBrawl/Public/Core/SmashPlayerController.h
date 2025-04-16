// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SmashPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ASmashPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASmashPlayerController();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Smash Ability")
	TSubclassOf<UUserWidget> WidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "SmashAbility")
	UUserWidget* WidgetPtr;
};
