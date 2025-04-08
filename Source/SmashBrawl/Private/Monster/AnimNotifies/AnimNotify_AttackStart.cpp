// 


#include "Monster/AnimNotifies/AnimNotify_AttackStart.h"

#include "Monster/BaseBossMonster.h"

void UAnimNotify_AttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ABaseBossMonster* BossMonster = Cast<ABaseBossMonster>(Owner);

		if (IsValid(BossMonster))
		{
			BossMonster->StartAttack();
		}
	}
}
