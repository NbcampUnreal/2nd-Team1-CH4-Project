// 

#pragma once

#include "CoreMinimal.h"
#include "BaseBossMonster.h"
#include "BaseBossPatternActor.h"
#include "Lv1BossMonster.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonster : public ABaseBossMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALv1BossMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ABaseBossPatternActor> BreathProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ABaseBossPatternActor> LavaBurst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ABaseBossPatternActor> SideLavaBurstR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ABaseBossPatternActor> SideLavaBurstL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ABaseBossPatternActor> Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ABaseBossPatternActor> Vacuum;
	
	UFUNCTION(Server, Reliable)
	void Server_BreathAttack() const;

	UFUNCTION(Server, Reliable)
	void Server_BiteAttack() const;

	UFUNCTION(Server, Reliable)
	void Server_FlyBreathAttack() const;

	UFUNCTION(Server, Reliable)
	void Server_LavaBurstAttack() const;

	UFUNCTION(Server, Reliable)
	void Server_MagicAttack();

	UFUNCTION(Server, Reliable)
	void Server_SideLavaRAttack();

	UFUNCTION(Server, Reliable)
	void Server_SideLavaLAttack();

	UFUNCTION(Server, Reliable)
	void Server_VacuumAttack();
};
