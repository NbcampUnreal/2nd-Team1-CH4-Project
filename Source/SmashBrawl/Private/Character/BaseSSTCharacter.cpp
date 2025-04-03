


#include "SmashBrawl/Public/Character//BaseSSTCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/BrawlPlayerController.h"
#include "SSTCharacterMovementComponent.h"
#include "AbilitySystem/SmashAbilitySystemComponent.h"
#include "Character/StateSystem.h"
#include "Net/UnrealNetwork.h"

class ABrawlPlayerController;

ABaseSSTCharacter::ABaseSSTCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<USmashAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CharacterState = CreateDefaultSubobject<UStateSystem>(TEXT("CharacterState"));
}

void ABaseSSTCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseSSTCharacter, bCanMove);
}

void ABaseSSTCharacter::Move(const struct FInputActionValue& Value)
{
	Super::Move(Value);
	if (Controller && SSTCharacterMovementComponent && bCanMove)
	{
		float MovementValue = Value.Get<float>();
		SSTCharacterMovementComponent->AddInputVector(FVector::ForwardVector * MovementValue);
		// 발소리 재생 로직 추가
		// if (CharacterState->GetCurrentState() == EPlayerStates::Idle && !bPushed && !MovementValue == 0.0f)
		// {
		// 	//FuncSetState 실행
		// 	CharacterState->ChangeCharacterState(EPlayerStates::WalkAndRun);
		// }
	}
}

void ABaseSSTCharacter::CrouchDrop()
{
	Super::CrouchDrop();
	if (CanCrouch())
	{
		Crouch();
	}
}

void ABaseSSTCharacter::StopCrouchDrop()
{
	Super::StopCrouchDrop();
	UnCrouch();
}

void ABaseSSTCharacter::JumpOrDrop()
{
	Super::JumpOrDrop();
	
	bJumpButton = true;
	
	if (bIsCrouched) // attempt to drop through platform, if any
	{
		SSTCharacterMovementComponent->WantsToPlatformDrop = true;
	}
	else
	{
		Jump();
	}
}

void ABaseSSTCharacter::Dash()
{
	Super::Dash();
	SSTCharacterMovementComponent->WantsToDash = true;
}

void ABaseSSTCharacter::ReleaseJump()
{
	Super::ReleaseJump();

	bJumpButton = false;
	
	StopJumping();
}

bool ABaseSSTCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() || SSTCharacterMovementComponent->CanWalljump();
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
