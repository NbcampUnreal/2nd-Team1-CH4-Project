


#include "../../Public/Monster/BaseBossMonster.h"

#include "Components/SphereComponent.h"

// Sets default values
ABaseBossMonster::ABaseBossMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HeadComp = CreateDefaultSubobject<USphereComponent>(TEXT("HeadComp"));
	HeadComp->SetupAttachment(RootComponent);

	bReplicates = true;

	

	
}
