// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_HUD_CharacterInfo.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UUW_HUD_CharacterInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="HUD Character Info")
	void PlayShake()
	{
		
	}
};
