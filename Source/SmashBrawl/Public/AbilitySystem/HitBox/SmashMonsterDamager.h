// 

#pragma once

#include "CoreMinimal.h"
#include "SmashBaseDamager.h"
#include "SmashMonsterDamager.generated.h"

UCLASS()
class SMASHBRAWL_API ASmashMonsterDamager : public ASmashBaseDamager
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmashMonsterDamager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
