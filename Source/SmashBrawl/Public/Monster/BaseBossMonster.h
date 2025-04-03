

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

	UFUNCTION(BlueprintPure, Category = "Anim")
	FORCEINLINE class UAnimMontage* GetMontage(int32 MontageIndex) const { return AnimMontages[MontageIndex]; }

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* StatsTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* HeadComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<class UAnimMontage*> AnimMontages;
};
