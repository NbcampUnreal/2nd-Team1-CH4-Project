

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Interface_BossMonsterCombat.h"
#include "BaseBossMonster.generated.h"

UCLASS()
class SMASHBRAWL_API ABaseBossMonster : public ACharacter, public IInterface_BossMonsterCombat
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseBossMonster();

	UFUNCTION(BlueprintPure, Category = "Anim")
	FORCEINLINE class UAnimMontage* GetMontage(int32 MontageIndex) const { return AnimMontages[MontageIndex]; }

	
	//이 함수는 RPC가 아님. 로컬에서도 PerformAttack() 호출 시 실행 가능
	virtual void PerformAttack_Implementation(int32 MontageIndex) override;

	//이 함수는 서버에서만 실행되어야 함. 실질적 공격 함수
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_PerformAnimation(UAnimMontage* Montage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* StatsTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* HeadCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<class UAnimMontage*> AnimMontages;

	UFUNCTION(BlueprintPure, Category = "Value")
	int32 GetRandomValueInMontageIndex() const;
	
	virtual void ReactToHit_Implementation() override;

	virtual  void OnDeath_Implementation() override;
};
