// 


#include "AbilitySystem/HitBox/SmashPlayerDamagerHitModel.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashStateSystem.h"
#include "Core/SmashTypes.h"


class ASmashBaseDamager;
// Sets default values for this component's properties
USmashPlayerDamagerHitModel::USmashPlayerDamagerHitModel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool USmashPlayerDamagerHitModel::bHitConditions(AActor* OtherActor, UActorComponent* OtherComp)
{
	if (ASmashCharacter* OtherCharacter = Cast<ASmashCharacter>(OtherActor))
	{
		if (OtherCharacter->StateSystem->GetCurrentState() != ESmashPlayerStates::Hit &&
			OtherCharacter->HitStates != ESmashHitState::Intangible)
		{
			return true;
		}
	}
	
	return false;
}


void USmashPlayerDamagerHitModel::Server_OverlapMesh_Implementation(AActor* Target, UPrimitiveComponent* OtherComp, bool bIsRightDirection)
{
	if (IInterface_SmashHitBoxPriority* PriorityDamager = Cast<IInterface_SmashHitBoxPriority>(OtherComp))
	{
		if (IInterface_SmashHitBoxPriority* OwnerPriorityDamager = Cast<IInterface_SmashHitBoxPriority>(GetOwner()))
		{
			OwnerPriorityDamager->ActionPriority(PriorityDamager->GetPriority());
			PriorityDamager->ActionPriority(OwnerPriorityDamager->GetPriority());

			if (OwnerPriorityDamager->ComparatorPriority(PriorityDamager->GetPriority()) <= 0)
			{
				return;
			}
		}
	}

	MultiCast_OverlapMesh();
	// 데미지 전달
	
}

void USmashPlayerDamagerHitModel::MultiCast_OverlapMesh_Implementation()
{
}

