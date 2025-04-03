


#include "SmashBrawl/Public/Character//BaseSSTCharacter.h"

#include "EnhancedInputComponent.h"
#include "Character/BrawlPlayerController.h"
#include "Character/StateSystem.h"

ABaseSSTCharacter::ABaseSSTCharacter(const FObjectInitializer& ObjectInitializer) : ASSTCharacter(ObjectInitializer)
{
	StateSystem = CreateDefaultSubobject<UStateSystem>(TEXT("StateSystem"));
}

void ABaseSSTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (ABrawlPlayerController* PlayerController = Cast<ABrawlPlayerController>(GetLocalViewingPlayerController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
		{
			//Jumping
			EnhancedInputComponent->BindAction(PlayerController->BasicAttackAction, ETriggerEvent::Triggered, this, &ABaseSSTCharacter::BaseAttack);
			EnhancedInputComponent->BindAction(PlayerController->BasicAttackAction, ETriggerEvent::Completed, this, &ABaseSSTCharacter::SpecialAttack);

		}
	}

}

void ABaseSSTCharacter::BaseAttack()
{
	StateSystem->ChangeCharacterState(EPlayerStates::Ability);
	
}

void ABaseSSTCharacter::SpecialAttack()
{
}
