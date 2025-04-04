#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashTypes.generated.h"


UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	WalkAndRun UMETA(DisplayName = "Walk/Run"),
	Crouch UMETA(DisplayName = "Crouch"),
	Sprint UMETA(DisplayName = "Sprint"),
	Jump UMETA(DisplayName = "Jump"),
	Fall UMETA(DisplayName = "Fall"),
	Hit UMETA(DisplayName = "Hit"),
	Launch UMETA(DisplayName = "Launch"),
	Ledge UMETA(DisplayName = "Ledge"),
	Dead UMETA(DisplayName = "Dead"),
	Shield UMETA(DisplayName = "Shield"),
	Tumble UMETA(DisplayName = "Tumble"),
	ShieldTumble UMETA(DisplayName = "Shield Tumble"),
	Prone UMETA(DisplayName = "Prone"),
	Stun UMETA(DisplayName = "Stun"),
	Dizzy UMETA(DisplayName = "Dizzy"),
	Ability UMETA(DisplayName = "Ability"),
	Dodge UMETA(DisplayName = "Dodge"),
	FreeFall UMETA(DisplayName = "Free Fall"),
	Held UMETA(DisplayName = "Held"),
	Hold UMETA(DisplayName = "Hold")
};


UENUM(BlueprintType)
enum class EAttacks : uint8
{
	TiltUp UMETA(DisplayName = "Tilt Up"),
	TiltDown UMETA(DisplayName = "Tilt Down"),
	TiltForward UMETA(DisplayName = "Tilt Forward"),
	TiltBack UMETA(DisplayName = "Tilt Back"),
	Neutral UMETA(DisplayName = "Neutral"),
	AirUp UMETA(DisplayName = "AirUp"),
	AirDown UMETA(DisplayName = "AirDown"),
	AirForward UMETA(DisplayName = "AirForward"),
	AirBack UMETA(DisplayName = "AirBack"),
	SpecialUp UMETA(DisplayName = "SpecialUp"),
	SpecialDown UMETA(DisplayName = "SpecialDown"),
	SpecialForward UMETA(DisplayName = "SpecialForward"),
	SpecialNeutral UMETA(DisplayName = "SpecialNeutral"),
	SmashUp UMETA(DisplayName = "SmashUp"),
	SmashDown UMETA(DisplayName = "SmashDown"),
	SmashForward UMETA(DisplayName = "SmashForward"),
	SmashBack UMETA(DisplayName = "SmashBack"),
	Grab UMETA(DisplayName = "Grab"),
	ThrowUp UMETA(DisplayName = "ThrowUp"),
	ThrowDown UMETA(DisplayName = "ThrowDown"),
	ThrowForward UMETA(DisplayName = "ThrowForward"),
	ThrowBack UMETA(DisplayName = "ThrowBack"),
	Pummel UMETA(DisplayName = "Pummel"),
	LedgeAttack UMETA(DisplayName = "Ledge Attack"),
	ProneAttack UMETA(DisplayName = "Prone Attack"),
	DodgeSpot UMETA(DisplayName = "DodgeSpot"),
	DodgeAir UMETA(DisplayName = "DodgeAir"),
	DodgeBack UMETA(DisplayName = "DodgeBack"),
	DodgeForward UMETA(DisplayName = "DodgeForward"),
	Super UMETA(DisplayName = "Super"),
	None UMETA(DisplayName = "None")
};


UENUM(BlueprintType)
enum class EAbilityTypes : uint8
{
	None UMETA(DisplayName = "None"),
	Basic UMETA(DisplayName = "Basic"),
	Special UMETA(DisplayName = "Special"),
	Air UMETA(DisplayName = "Air"),
	Smash UMETA(DisplayName = "Smash"),
	Super UMETA(DisplayName = "Super"),
	Dodge UMETA(DisplayName = "Dodge"),
	Taunt UMETA(DisplayName = "Taunt"),
	Ledge UMETA(DisplayName = "Ledge"),
	Throw UMETA(DisplayName = "Throw"),
	Prone UMETA(DisplayName = "Prone"),
	Grab UMETA(DisplayName = "Grab"),
	Other UMETA(DisplayName = "Other")
};

UENUM(BlueprintType)
enum class ESmashBuffer : uint8
{
	Attack UMETA(DisplayName = "Attack"),
	Special UMETA(DisplayName = "Special"),
	Jump UMETA(DisplayName = "Jump"),
	Shield UMETA(DisplayName = "Shield"),
	None UMETA(DisplayName = "None")
};


UENUM(BlueprintType)
enum class ESmashDirection : uint8
{
	Up UMETA(DisplayName = "Up"),
	Down UMETA(DisplayName = "Down"),
	Back UMETA(DisplayName = "Back"),
	Forward UMETA(DisplayName = "Forward"),
	None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ESmashBossState : uint8
{
	Phase1 UMETA(DisplayName = "Phase1"),
	ChangingPhase UMETA(DisplayName = "ChangingPhase"),
	Phase2 UMETA(DisplayName = "Phase2"),
	Dead UMETA(DisplayName = "Dead"),
};