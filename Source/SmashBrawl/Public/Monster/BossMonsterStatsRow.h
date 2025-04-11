

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BossMonsterStatsRow.generated.h"

USTRUCT(BlueprintType)
struct FBossMonsterStatsRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterDamage;
};


// class SMASHBRAWL_API BossMonsterStatsRow
// {
// public:
// 	BossMonsterStatsRow();
// 	~BossMonsterStatsRow();
// };
