// 


#include "Monster/AnimNotifies/AnimNotify_EndGame.h"

#include "Core/SmashPlatFighterGameMode.h"

void UAnimNotify_EndGame::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameModeInstance = World->GetAuthGameMode())
		{
			if (ASmashPlatFighterGameMode* GameMode = Cast<ASmashPlatFighterGameMode>(GameModeInstance))
			{
				GameMode->Multicast_EndGame();
			}
		}
	}
}
