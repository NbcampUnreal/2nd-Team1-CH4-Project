// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/SmashCharacter.h"
#include "UW_HUD_PlayerName.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UUW_HUD_PlayerName : public UUserWidget
{
	GENERATED_BODY()



	//네트워크 확인하세요
public:
	UPROPERTY()
	ASmashCharacter* Player;
	
	UPROPERTY()
	int32 PlayerNo;
};
