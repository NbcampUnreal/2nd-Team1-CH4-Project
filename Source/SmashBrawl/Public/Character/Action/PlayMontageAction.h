// 

#pragma once

#include "CoreMinimal.h"
#include "SmashAction.h"
#include "PlayMontageAction.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UPlayMontageAction : public USmashAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	bool bAninEndNext;
	UPROPERTY(EditAnywhere)
	ESmashPlayerStates nextState;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
};
