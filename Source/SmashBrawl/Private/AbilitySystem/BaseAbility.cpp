// 


#include "AbilitySystem/BaseAbility.h"

#include <Character/Components/SmashStateSystem.h>


#include "AbilitySystem/SmashAbilitySystemComponent.h"
#include "AbilitySystem/HitBox/SmashAbilityDamagerManager.h"
#include "Net/UnrealNetwork.h"
#include "Character/SmashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Interface_SmashCombat.h"
#include "Components/ActorComponent.h"

// Sets default values
ABaseAbility::ABaseAbility()
{
	PrimaryActorTick.bCanEverTick = true;
	MainTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MainTimeline"));
	SmashAbilityDamagerManager = CreateDefaultSubobject<USmashAbilityDamagerManager>(TEXT("DamagerManager"));

	AttackContain.Add(FName(TEXT("Attack1")));
	AttackContain.Add(FName(TEXT("Attack2")));
	AttackContain.Add(FName(TEXT("Attack3")));
	AttackContain.Add(FName(TEXT("Attack4")));
	AttackContain.Add(FName(TEXT("None")));

	CollisionSet = 1;
	
	// PlatformFighterKit/Blueprints/Projectiles/BP_ProjectileBase_C
	// ConstructorHelpers::FClassFinder<>

	//AttackData = // ConstructorHelpers::FObjectFinder<>
	
}

// Called when the game starts or when spawned
void ABaseAbility::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(Parent))
	{
		//AttackData = Parent(BP_Fighter�� AttackData)
		StartTimeline();
	}
	else
	{
	}
}

void ABaseAbility::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseAbility, Parent);
	DOREPLIFETIME(ABaseAbility, CollisionSetArray);
	DOREPLIFETIME(ABaseAbility, bActive);
	DOREPLIFETIME(ABaseAbility, bIsUse);
	DOREPLIFETIME(ABaseAbility, bLanding);
	DOREPLIFETIME(ABaseAbility, Direction);
	DOREPLIFETIME(ABaseAbility, BufferDirection);
	DOREPLIFETIME(ABaseAbility, Projectile);
}

void ABaseAbility::Multicast_AbilityStart_Implementation()
{
	BP_OnAbilityStart();
}

// void ABaseAbility::StartTimeline()
// {
// 	if (MainTimeline)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("StartTimeLine"));
// 		MainTimeline->PlayFromStart();
// 	}
// }

// void ABaseAbility::EndTimeline()
// {
// 	if (MainTimeline)
// 	{
// 		MainTimeline->Stop();
// 	}
// }

void ABaseAbility::Multicast_TickRep_Implementation()
{
		if (bActive)
		{
			ActiveLoop();
			FuncCharging();
			SpecialAttackButton();
			if (Parent && Parent->GetCapsuleComponent())
			{
				ParentLocation = Parent->GetCapsuleComponent()->GetComponentLocation();
				SetBuffer();
				if (!bIsUse)
				{
					UE_LOG(LogTemp, Log, TEXT("StartAbility"));
					Multicast_AbilityStart_Implementation();
					bIsUse = true;
				}
				//Then1
				if (Parent && Parent->bLanded)
				{
					if (!bLandClosed)
					{
						
						bLandClosed = true;
						bLanding = true;
						Land();
					}
				}
				else
				{
					bLandClosed = false;
				}
				//Then2
				if (false) //Character�� �ɹ����� HitButNoEffect(bool)�� ���
				{
					if (!bTakeAHitClosed)
					{
						TakeAHit();
						bTakeAHitClosed = true;
					}
				}
				else
				{
					bTakeAHitClosed = false;
				}
				//Then3
				if (IInterface_SmashCombat::Execute_GetPlayerState(Parent) == ESmashPlayerStates::Dead || IInterface_SmashCombat::Execute_GetPlayerState(Parent) == ESmashPlayerStates::Hit) //Character�� States(Enum)�� ���
				{
					Multicast_EndAbility();
				}
				//Then4
				if (IInterface_SmashCombat::Execute_GetPlayerState(Parent) == ESmashPlayerStates::Ledge && !bDontEndOnLedge) //Character�� States(Enum)�� ���
				{
					Multicast_EndAbility();
				}
				//Then5
				if (false) //
				{
					if (!bHitByProjectileClosed)
					{
						bHitByProjectileClosed = true;
						HitByProjectile(10, false); 
					}
				}
				else
				{
					bHitByProjectileClosed = false;
				}
			}
		}
}

void ABaseAbility::Multicast_SoftEnd_Implementation()
{
	WalkOffLedge(true);
	bActive = false;
	Multicast_SetFlip(true);
	SetJump(true);
	SetMovement(true);
	SetCanAttack(true);
	RemoveDamagers();
	AbsorbMode(false);
	ReflectMode(false);
	bIsUse = false;
	bInputButter = false;
}

void ABaseAbility::Multicast_EndAbility_Implementation()
{
	BP_OnEndAbility();
}

void ABaseAbility::BP_OnEndAbility_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("EndAbility"));
	WalkOffLedge(true);
	bActive = false;
	Multicast_SetFlip(true);
	SetJump(true);
	SetMovement(true);
	CharacterCollision(ECollisionEnabled::Type::QueryAndPhysics);
	FullbodyLedge(false);
	bIsUse = false;
	if(Parent->GetMovementComponent()->IsFalling())
	{
		Parent->SmashStateSystem->TryChangeState(ESmashPlayerStates::Fall,false);
	}
	else
	{
		Parent->SmashStateSystem->TryChangeState(ESmashPlayerStates::Idle,false);
	}
	Parent->AbilityType = ESmashAbilityTypes::None;
	IInterface_SmashCombat::Execute_SetAttacks(Parent,ESmashAttacks::None);
	if (bInputButter)
	{
		bInputButter = false;
		//Parent->StopInputBuffer();
	}
}

void ABaseAbility::Reset()
{
	ChargeLevel = 0;
	ChargeRatio = 0;
	bCharging = false;
	bCharged = false;
	bIsUse = false;
	AnimNo = 1;
	CollisionSet = 1;
	bActivateCollision = false;
	WalkOffLedge(true);
	CollisionSetIndex = 0;
}

void ABaseAbility::Multicast_LandLeg_Implementation(UAnimMontage* LandAnimation, float PlayRate, float StartPosition,
                                                    FName StartSection, int32 InAnimNo)
{
	Multicast_SetFlip(false);
	SetJump(false);
	SetMovement(false);
	UAnimInstance* AnimInstance = Parent->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(LandAnimation, PlayRate);
		if (StartPosition > 0.0f)
		{
			AnimInstance->Montage_SetPosition(LandAnimation, StartPosition);
		}

		if (StartSection != NAME_None)
		{
			AnimInstance->Montage_JumpToSection(StartSection, LandAnimation);
		}
		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindLambda([this, InAnimNo,LandAnimation](UAnimMontage* Montage, bool bInterrupted)
		{
			if (Montage == LandAnimation)
			{
				Multicast_SetFlip(true);
				SetJump(true);
				SetMovement(true);
				Multicast_EndAnim(InAnimNo);
				bLanding = false;
				Multicast_EndAbility();
			}
		});
		AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, LandAnimation);
	}
}

void ABaseAbility::ActivateDamagers_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ActivateDamagers"));
	if (HasAuthority())
	{
		Server_AddDamagersServer(Parent);
	}
	else
	{
		Multicast_AddDamagersClient(Parent);
	}
}

void ABaseAbility::FuncDamageBoxes(ACharacter* InParent)
{
	ChildDamagers = SmashAbilityDamagerManager->SpawnDamagerAll();
	for (AActor* ChildDamager : ChildDamagers)
	{
		ChildDamager->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ABaseAbility::Multicast_AddDamagersClient_Implementation(ACharacter* InParent)
{
	FuncDamageBoxes(InParent);
}

void ABaseAbility::Server_AddDamagersServer_Implementation(ACharacter* InParent)
{
	FuncDamageBoxes(InParent);
}

void ABaseAbility::Server_SpawnProjectile_Implementation(TSubclassOf<AActor> ProjectileClass, FTransform SpawnTransform,
                                                         float Lifetime, float YSpeed, float ZSpeed, float Gravity,
                                                         FName AttackName, float DamageMultiply)
{
	if (ProjectileClass)
	{
		AActor* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AActor>(
			ProjectileClass,
			SpawnTransform
		);
		SpawnedProjectile->Owner = Parent;
		SpawnedProjectile->SetInstigator(Parent);
		//Set Projectile Info
		FinishSpawning(SpawnTransform,true);
	}
}

void ABaseAbility::Multicast_Throw_Implementation(FName Name)
{
	
}

void ABaseAbility::Multicast_ReShield_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(ReShieldTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		//Parent�� ShieldButton(bool)�� ���� Set Notify
	}), 0.01, false);
}

void ABaseAbility::RemoveDamagers()
{
	//TArray<AActor*> Damagers;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Damagers);
	//// AActor::StaticClass�� BP_Damager Ŭ������ ����
	//for (AActor* Damager : Damagers)
	//{
	//	if (Damager->GetParentActor() == Parent)
	//	{
	//		Damager->Destroy();
	//	}
	//}
	for (AActor* ChildDamager : ChildDamagers)
	{
		ChildDamager->Destroy();
	}
}

void ABaseAbility::FlipOnStick()
{
}

void ABaseAbility::PlayAnimation(int32 InAnimNo)
{
	Server_PlayAnimationServer(InAnimNo, Animations[InAnimNo]);
}

void ABaseAbility::Multicast_PlayAnimationClient_Implementation(int32 InAnimNo, UAnimMontage* InMontageToPlay)
{
	UAnimInstance* AnimInstance = Parent->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(InMontageToPlay, 1.0f);
		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindLambda([this, InAnimNo,AnimInstance](UAnimMontage* Montage, bool bInterrupted)
		{
			UE_LOG(LogTemp, Error, TEXT("EndPlayAnim_Multicast"));
			Multicast_EndAnim(InAnimNo);
			if (bInterrupted)
			{
				AnimNotifyEnd(bDamager, "None");
				UE_LOG(LogTemp, Error, TEXT("EndPlayAnim_Multicast_Interrupted"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("EndPlayAnim_Multicast_Not_Interrupted"));
			}
			AnimInstance->OnPlayMontageNotifyBegin.Clear();
			AnimInstance->OnPlayMontageNotifyEnd.Clear();
		});
		AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, InMontageToPlay);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ABaseAbility::OnNotifyBegin);
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &ABaseAbility::OnNotifyEnd);
	}
}

void ABaseAbility::Server_PlayAnimationServer_Implementation(int32 InAnimNo, UAnimMontage* InMontageToPlay)
{
	Multicast_PlayAnimationClient(InAnimNo, Animations[InAnimNo]);
}

void ABaseAbility::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(LogTemp, Error, TEXT("%s"),*NotifyName.ToString());
	if (NotifyName == "None" || AttackContain.Contains(NotifyName))
	{
		bDamager = true;
	}
	else
	{
		bDamager = false;
	}
	AnimNotifyStart(bDamager, NotifyName);
}

void ABaseAbility::OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UAnimInstance* AnimInstance = Parent->GetMesh()->GetAnimInstance();
	AnimNotifyEnd(bDamager, NotifyName);
}

void ABaseAbility::AnimNotifyStart_Implementation(bool InDamager, FName NoteName)
{
	if (InDamager)
	{
		ActivateDamagers();
		UE_LOG(LogTemp, Log, TEXT("ActivateDamager"));
	}
	if (NoteName == "ChangSet")
	{
		ChangeCollisionSet();
	}
	if (NoteName == "Buffer")
	{
		bInputButter = true;
	}
	else
	{
		bInputButter = false;
	}
	if (NoteName == "Counter")
	{
		HitStates(ESmashHitState::Intangible);
	}
	if (NoteName == "Event")
	{
		AbilityEvent();
	}
}

void ABaseAbility::AnimNotifyEnd(bool InDamager, FName NoteName)
{
	if (InDamager)
		RemoveDamagers();
}


void ABaseAbility::DebugAbilityCall(int32 PlayerNumber, bool bDebug)
{
	//UE_LOG(LogTemp, Log, TEXT("DebugAbilityCall Executed - Player: %d, Debug: %s"), PlayerNumber, Debug ? TEXT("True") : TEXT("False"));
}

void ABaseAbility::FuncCharging()
{
	if (!bCharging)
	{
		return;
	}
	ChargeLevel += ChargeRate;
	ChargeRatio = FMath::Clamp((ChargeLevel / ChargeMax), 0, 1.0f);
	if (ChargeLevel >= ChargeMax)
	{
		ChargeLevel = ChargeMax;
		bCharged = true;
	}
}

void ABaseAbility::SpecialAttackButton()
{
	if (!Parent)
		return;
	if (Parent->bSpecialAttackButton)
	{
		SpecialButtonDown();
	}
	else
	{
		SpecialButtonUp();
	}
}

void ABaseAbility::SetBuffer()
{
	if (bInputButter)
	{
		bBufferClosed = false;
		//ĳ������ StartInputBuffer ȣ��
	}
	else
	{
		if (!bBufferClosed)
		{
			bBufferClosed = true;
			//ĳ������ StopInputBufferȣ��
		}
	}
}

void ABaseAbility::SetMovement(bool bCanMove)
{
	if (Parent)
	{
		FSmashPlayerStateInfo SmashPlayerStateInfo = Parent->GetStateInfo();
		SmashPlayerStateInfo.PlayerMovement.bCanMove = bCanMove;
		Parent->SetStateInfo(SmashPlayerStateInfo);
	}
}

void ABaseAbility::SetCanAttack(bool bCanAttack)
{
	if (Parent)
	{
		FSmashPlayerStateInfo SmashPlayerStateInfo = Parent->GetStateInfo();
		SmashPlayerStateInfo.PlayerMovement.bCanAttack = bCanAttack;
		Parent->SetStateInfo(SmashPlayerStateInfo);
	}
}

void ABaseAbility::SetJump(bool bCanJump)
{
	if (Parent)
	{
		FSmashPlayerStateInfo SmashPlayerStateInfo = Parent->GetStateInfo();
		SmashPlayerStateInfo.PlayerMovement.bCanJump = bCanJump;
		Parent->SetStateInfo(SmashPlayerStateInfo);
	}
}

void ABaseAbility::DodgeDelay()
{
	// DodgeDelayEvent �Լ� ȣ��
}

void ABaseAbility::Multicast_SetFlip_Implementation(bool bCanFlip)
{
	if (!Parent)
		return;
	FSmashPlayerStateInfo SmashPlayerStateInfo = Parent->GetStateInfo();
	SmashPlayerStateInfo.PlayerMovement.bCanFlipping = bCanFlip;
	Parent->SetStateInfo(SmashPlayerStateInfo);
}

void ABaseAbility::CharacterCollision(ECollisionEnabled::Type Type)
{
	if (!Parent)
		return;
	Parent->GetCapsuleComponent()->SetCollisionEnabled(Type);
}

void ABaseAbility::WalkOffLedge(bool bCanWalkOff)
{
	if (!Parent)
		return;
	Parent->GetCharacterMovement()->bCanWalkOffLedges = bCanWalkOff;
}

void ABaseAbility::Multicast_EndAnim_Implementation(int32 InAnimNo)
{
	BP_OnEndAnim(InAnimNo);
}

void ABaseAbility::ActivateOtherAbility(ABaseAbility* Ability)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAbility::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (Parent->GetOwner() == Actor || Ability == Actor)
		{
		}
	}
}

void ABaseAbility::HitStates(ESmashHitState SmashHitState)
{
	if (Parent)
	{
		Parent->HitStates = SmashHitState;
	}
}

void ABaseAbility::SetAttackStage(int32 _AttackStage)
{
	AttackStage = _AttackStage;
}

void ABaseAbility::ChangeCollisionSet()
{
	CollisionSetIndex++;
	CollisionSet = CollisionSets[CollisionSetIndex];
}

void ABaseAbility::AbsorbMode(bool bAbsorb)
{
}

void ABaseAbility::ReflectMode(bool bReflect)
{
}

void ABaseAbility::FullbodyLedge(bool bFullBodyLedgeGrab)
{
	
}

void ABaseAbility::TeleportCharacter(FVector Location, bool bSweep, bool bTeleport)
{
	Parent->GetCapsuleComponent()->SetWorldLocation(Location, bSweep);
}

void ABaseAbility::Multicast_StartParticles_Implementation(bool bAttached, UParticleSystem* EmitterTemplate,
                                                           USceneComponent* AttachToComponent, FName AttachPointName,
                                                           FVector Location, FRotator Rotation, FVector Scale,
                                                           EAttachLocation::Type LocationType, bool bAutoDestroy)
{
	if (!EmitterTemplate)
		return;
	if (bAttached && AttachToComponent)
	{
		UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, AttachToComponent, AttachPointName,
		                                       Location, Rotation, LocationType, bAutoDestroy);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, Location, Rotation, bAutoDestroy);
	}
}

void ABaseAbility::SetGravity(int32 Settings, float CustomValue)
{
	if (!Parent)
		return;
	switch (Settings)
	{
	case 1:
		Parent->GetCharacterMovement()->GravityScale = 1.0f;
		break;
	case 2:
		Parent->GetCharacterMovement()->GravityScale = 0.0f;
		break;
	case 3:
		Parent->GetCharacterMovement()->GravityScale = CustomValue;
		break;
	default: break;
	}
}

void ABaseAbility::HealPlayer(int32 HealAmount)
{
	
}

void ABaseAbility::Charge(bool InChargeing)
{
	bCharging = InChargeing;
}

void ABaseAbility::EndAllNonChargedAbilities(ABaseAbility* Caller)
{
	Parent->AbilitySystemComponent->EndAllNonChargedAbilities(Caller);
}

void ABaseAbility::StopAllMovement()
{
	if (!Parent)
		return;
	Parent->GetMovementComponent()->StopMovementImmediately();
}