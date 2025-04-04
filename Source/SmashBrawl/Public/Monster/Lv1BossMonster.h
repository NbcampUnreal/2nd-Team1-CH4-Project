// 

#pragma once

#include "CoreMinimal.h"
#include "BaseBossMonster.h"
#include "Monster/Lv1BossMonsterBreathProjectile.h"
#include "Lv1BossMonster.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonster : public ABaseBossMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALv1BossMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ALv1BossMonsterBreathProjectile> BreathProjectile;
	
	UFUNCTION(Server, Reliable)
	void Server_BreathAttack() const;

	UFUNCTION(Server, Reliable)
	void Server_BiteAttack() const;

	UFUNCTION(Server, Reliable)
	void Server_FlyBreathAttack() const;
};
