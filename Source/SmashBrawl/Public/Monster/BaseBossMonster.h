

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseBossMonster.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseBossMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseBossMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* StatsTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* HeadComp;
};
