// 

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacter.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "SmashGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API USmashGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SmashGameInstance")
	TArray<bool> CPU;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SmashGameInstance")
	TArray<ESmashCharacter> PlayerCharacters;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SmashGameInstance")
	TArray<bool> AliveArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SmashGameInstance")
	TArray<int32> PlayerTeam;
};
