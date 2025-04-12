

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnLocation.generated.h"

UCLASS()
class SMASHBRAWL_API ARespawnLocation : public AActor
{
	GENERATED_BODY()
	
public:
	ARespawnLocation();

protected:
	virtual void BeginPlay() override;
};

