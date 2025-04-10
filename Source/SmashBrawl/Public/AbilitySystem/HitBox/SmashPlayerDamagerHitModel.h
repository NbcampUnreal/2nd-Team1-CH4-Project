// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_SmashHitBox.h"
#include "SmashPlayerDamagerHitModel.generated.h"


class ASmashBaseDamager;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashPlayerDamagerHitModel : public UActorComponent, public IInterface_SmashHitBox
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashPlayerDamagerHitModel();

	UFUNCTION()
	virtual bool bHitConditions(AActor* OtherActor, UActorComponent* OtherComp) override;
	UFUNCTION(Server, Reliable)
	virtual void Server_OverlapMesh(AActor* Target, UPrimitiveComponent* OtherComp, bool bIsRightDirection) override;
	UFUNCTION(NetMulticast, UnReliable)
	virtual void MultiCast_OverlapMesh() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<ASmashBaseDamager> Owner;
};
