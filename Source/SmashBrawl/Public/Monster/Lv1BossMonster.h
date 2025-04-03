// 

#pragma once

#include "CoreMinimal.h"
#include "BaseBossMonster.h"
#include "Interface/Interface_BossMonsterCombat.h"

#include "Monster/Lv1BossMonsterBreathProjectile.h"
#include "Lv1BossMonster.generated.h"

UCLASS()
class SMASHBRAWL_API ALv1BossMonster : public ABaseBossMonster, public IInterface_BossMonsterCombat
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALv1BossMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ALv1BossMonsterBreathProjectile> BreathProjectile;

	//이 함수는 RPC가 아님. 로컬에서도 PerformAttack() 호출 시 실행 가능
	virtual void PerformAttack_Implementation(UAnimMontage* Montage) override;

	//이 함수는 서버에서만 실행되어야 함. 실질적 공격 함수
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_PerformAnimation(UAnimMontage* Montage);

	UFUNCTION(Server, Reliable)
	void Server_BreathAttack() const;

	virtual void ReactToHit_Implementation() override;

	virtual  void OnDeath_Implementation() override;
};
