// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BrawlPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ABrawlPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABrawlPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* BasicAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* SpecialAttackAction;

	virtual void BeginPlay() override;

};
