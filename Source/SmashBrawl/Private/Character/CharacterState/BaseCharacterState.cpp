#include "Character/CharacterState/BaseCharacterState.h"

#include "Character/StateSystem.h"
#include "Character/BaseSSTCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

UBaseCharacterState::UBaseCharacterState()
{
	PlayerState = EPlayerStates::Idle;
}

void UBaseCharacterState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(UBaseCharacterState, PlayerStateInfo.bCanAttack);
}

void UBaseCharacterState::InitState_Implementation(UStateSystem* InStateSystem)
{
	OwnerStateSystem = InStateSystem;
	OwnerCharacter = Cast<ABaseSSTCharacter>(InStateSystem->GetOwner());
}

void UBaseCharacterState::EnterState_Implementation()
{
}

void UBaseCharacterState::ExitState_Implementation()
{
}

void UBaseCharacterState::TickState_Implementation()
{
}

bool UBaseCharacterState::CanState_Implementation()
{
	return true;
}

void UBaseCharacterState::SetCapsuleSize(float HalfHeight, bool bUpdateOverlap)
{
	OwnerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHeight, bUpdateOverlap);
}

EPlayerStates UBaseCharacterState::GetPlayerState() const
{
	return PlayerState;
}
