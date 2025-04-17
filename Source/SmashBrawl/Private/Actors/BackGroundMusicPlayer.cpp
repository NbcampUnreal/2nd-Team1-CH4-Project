// 


#include "Actors/BackGroundMusicPlayer.h"

#include "Components/AudioComponent.h"


// Sets default values
ABackGroundMusicPlayer::ABackGroundMusicPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bIsUISound = true;
	AudioComponent->bAllowSpatialization = false;
	AudioComponent->SetVolumeMultiplier(1.0f);
}

// Called when the game starts or when spawned
void ABackGroundMusicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (AudioComponent && Sound)
	{
		AudioComponent->SetSound(Sound);
		AudioComponent->FadeIn(2.0f, 1.0f);
	}
}