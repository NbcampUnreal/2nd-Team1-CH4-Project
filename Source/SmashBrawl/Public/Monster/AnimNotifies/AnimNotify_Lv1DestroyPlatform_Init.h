// 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Lv1DestroyPlatform_Init.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UAnimNotify_Lv1DestroyPlatform_Init : public UAnimNotify
{
	GENERATED_BODY()
			
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
