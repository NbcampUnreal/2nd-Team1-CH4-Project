// 


#include "SmashBrawl/Public/Charactor/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "Character/StateSystem.h"
#include "Charactor/BrawlPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter() :
bCanAttack(true), bCanSmash(true),AbilityTypes(EAbilityTypes::Basic),
bAttackButton(false), bAttackButtonReleased(true)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StateSystem = CreateDefaultSubobject<UStateSystem>(TEXT("StateSystem"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ABrawlPlayerController* PlayerController = Cast<ABrawlPlayerController>(GetController()))
		{
			if (PlayerController->BasicAttackAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->BasicAttackAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::BasicAttack);
			}
			if (PlayerController->BasicAttackAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->BasicAttackAction,
					ETriggerEvent::Completed,
					this,
					&ABaseCharacter::BasicAttackEnd);
			}
			if (PlayerController->SpecialAttackAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->SpecialAttackAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::SpecialAttack);
			}
		}
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, States);
	DOREPLIFETIME(ABaseCharacter, bCanAttack);
	DOREPLIFETIME(ABaseCharacter, bCanSmash);
	DOREPLIFETIME(ABaseCharacter, AbilityTypes);
}

void ABaseCharacter::BasicAttack()
{
	bAttackButton = true;
	if (States != EPlayerStates::Ability && bCanAttack)
	{
		Server_BasicAttack();
	}
	bAttackButtonReleased = false;
}

void ABaseCharacter::BasicAttackEnd()
{
	bAttackButtonReleased = true;
}

void ABaseCharacter::SpecialAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("SpecialAttack()"));

}

void ABaseCharacter::Server_BasicAttack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Do Attack1"));
	if (States == EPlayerStates::Shield)
	{
		States = EPlayerStates::Ability;
		AbilityTypes = EAbilityTypes::Grab;
	}
	else if (States != EPlayerStates::Ability && bCanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Do Attack2"));

		if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
		{
			AbilityTypes = EAbilityTypes::Air;
		}
		else if (bCanSmash)
		{
			AbilityTypes = EAbilityTypes::Smash;
			bCanSmash = false;
		}
		else if (States == EPlayerStates::Sprint)
		{
			AbilityTypes = EAbilityTypes::Other;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Do Attack3"));

			AbilityTypes = EAbilityTypes::Basic;
		}
		States = EPlayerStates::Ability;
	}
}

