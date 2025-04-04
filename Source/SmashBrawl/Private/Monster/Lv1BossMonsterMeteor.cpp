// 


#include "Monster/Lv1BossMonsterMeteor.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALv1BossMonsterMeteor::ALv1BossMonsterMeteor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComp);

	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ALv1BossMonsterMeteor::OnProjectileOverlapped);
	}

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComp);
	ProjectileComp->InitialSpeed = 1000;
	ProjectileComp->MaxSpeed = 1000;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->ProjectileGravityScale = 0.0f;

	Damage = 10.0f;

	Tags.Add("Projectile");

	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;
}

void ALv1BossMonsterMeteor::BeginPlay()
{
	Super::BeginPlay();
	
	if (NiagaraEffectTemplate)
	{
		NiagaraTrailEffect->Activate();
		NiagaraTrailEffect->SetAsset(NiagaraEffectTemplate);
	}
}

void ALv1BossMonsterMeteor::OnProjectileOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                             bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor)
	{
		if (!OtherActor->ActorHasTag(TEXT("Boss")) && !OtherActor->ActorHasTag(TEXT("Projectile")))
		{
			UE_LOG(LogTemp, Warning, TEXT("TT"));
			//UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorForwardVector(), SweepResult, nullptr, this, nullptr);
			Destroy();
		}
	}
}

void ALv1BossMonsterMeteor::Destroyed()
{
	Super::Destroyed();

	if (NiagaraDestroyEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraDestroyEffectTemplate,
			GetActorLocation(),
			GetActorRotation(),
			FVector(5.0f, 5.0f, 5.0f) 
		);
	}
}
