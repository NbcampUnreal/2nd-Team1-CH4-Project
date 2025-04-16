// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "BrawlPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ABrawlPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> OnlineTestWidgetClass;

	UPROPERTY()
	UUserWidget* OnlineTestWidget;
};
