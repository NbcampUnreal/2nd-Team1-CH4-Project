// 


#include "Monster/Lv1Phase1Platform.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BaseBossMonster.h"


// Sets default values
ALv1Phase1Platform::ALv1Phase1Platform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComp);
	BoxComp->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));

	bReplicates = true;
}

void ALv1Phase1Platform::BeginPlay()
{
	Super::BeginPlay();

	if (ABaseBossMonster* BossMonster = Cast<ABaseBossMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseBossMonster::StaticClass())))
	{
		BossMonster->OnTriggerDestruction.AddDynamic(this, &ALv1Phase1Platform::Destruction);
	}
}

void ALv1Phase1Platform::Destruction()
{
	Destroy();
}
