// 


#include "Monster/AnimNotifies/AnimNotify_Lv1DestroyPlatform_Init.h"

#include "Monster/Lv1BossMonster.h"

void UAnimNotify_Lv1DestroyPlatform_Init::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ALv1BossMonster* BossMonster = Cast<ALv1BossMonster>(Owner);

		if (IsValid(BossMonster))
		{
			BossMonster->Server_PlatformDestroy_Init();
		}
	}
}
