// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/DamageTable.h"
#include "Interfaces/Interface_SmashHitBox.h"
#include "SmashPlayerDamagerHitModel.generated.h"


class ASmashBaseDamager;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashPlayerDamagerHitModel : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashPlayerDamagerHitModel();

	virtual void BeginPlay() override;
};
