// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackGroundMusicPlayer.generated.h"

UCLASS()
class SMASHBRAWL_API ABackGroundMusicPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABackGroundMusicPlayer();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BackGround Music")
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BackGround Music")
	class USoundBase* Sound;
};
