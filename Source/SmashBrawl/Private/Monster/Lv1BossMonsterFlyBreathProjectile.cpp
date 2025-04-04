// 


#include "Monster/Lv1BossMonsterFlyBreathProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALv1BossMonsterFlyBreathProjectile::ALv1BossMonsterFlyBreathProjectile()
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
		SphereComp->OnComponentHit.AddDynamic(this, &ALv1BossMonsterFlyBreathProjectile::OnProjectileOverlapped);
	}

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComp);
	ProjectileComp->InitialSpeed = 500;
	ProjectileComp->MaxSpeed = 500;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->ProjectileGravityScale = 0.0f;

	Damage = 10.0f;
}

void ALv1BossMonsterFlyBreathProjectile::OnProjectileOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, nullptr);
	}
	Destroy();
}

void ALv1BossMonsterFlyBreathProjectile::Destroyed()
{
	Super::Destroyed();

	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), FRotator::ZeroRotator, true);
	}
}
