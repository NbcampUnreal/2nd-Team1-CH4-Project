// 


#include "Monster/AnimNotifies/AnimNotify_AttackEnd.h"

#include "Monster/BaseBossMonster.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ABaseBossMonster* BossMonster = Cast<ABaseBossMonster>(Owner);

		if (IsValid(BossMonster))
		{
			BossMonster->EndAttack();
		}
	}
}