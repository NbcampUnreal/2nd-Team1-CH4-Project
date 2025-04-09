// 


#include "Monster/Lv1BossMonsterBreathProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALv1BossMonsterBreathProjectile::ALv1BossMonsterBreathProjectile()
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
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ALv1BossMonsterBreathProjectile::OnProjectileOverlapped);
	}

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComp);
	ProjectileComp->InitialSpeed = 700;
	ProjectileComp->MaxSpeed = 700;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->ProjectileGravityScale = 0.0f;

	Damage = 10.0f;

	Tags.Add("Projectile");

	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->bAutoActivate = false;
}

void ALv1BossMonsterBreathProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraStartEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraStartEffectTemplate,
			GetActorLocation(),
			GetActorRotation(),
			FVector(5.0f, 5.0f, 5.0f) 
		);
	}

	
	if (NiagaraEffectTemplate)
	{
		NiagaraTrailEffect->Activate();
		NiagaraTrailEffect->SetAsset(NiagaraEffectTemplate);
	}
	
}

void ALv1BossMonsterBreathProjectile::OnProjectileOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                             bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor)
	{
		if (!OtherActor->ActorHasTag(TEXT("Boss")) && !OtherActor->ActorHasTag(TEXT("Projectile")))
		{
			//UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorForwardVector(), SweepResult, nullptr, this, nullptr);
			Destroy();
		}
	}
}

void ALv1BossMonsterBreathProjectile::Destroyed()
{
	Super::Destroyed();

	if (NiagaraStartEffectTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraStartEffectTemplate,
			GetActorLocation(),
			GetActorRotation(),
			FVector(5.0f, 5.0f, 5.0f) 
		);
	}
	
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
