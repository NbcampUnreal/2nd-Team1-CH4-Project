#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashTypes.generated.h"


// 공격 유형 열거형
UENUM(BlueprintType)
enum class ESmashAttackType : uint8
{
	Normal, // 일반 공격 (피격게이지만 증가)
	Knockback // 넉백 공격 (피격게이지 증가 + 날려보냄)
};


UENUM(BlueprintType)
enum class ESmashActionType : uint8
{
	None UMETA(DisplayName = "None"),
	PlayMontage UMETA(DisplayName = "PlayMontage"),
	// 필요한 액션들 추가...
};

UENUM(BlueprintType)
enum class ESmashInGameMode : uint8
{
	Stock UMETA(DisplayName = "Stock"),
	Time UMETA(DisplayName = "Time"),
	MiniGame UMETA(DisplayName = "MiniGame"),
	BossFight UMETA(DisplayName = "BossFight")
};

UENUM(BlueprintType)
enum class ESmashHitState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Intangible UMETA(DisplayName = "Intangible"),
	Invincible UMETA(DisplayName = "Invincible"),
	Armor UMETA(DisplayName = "Armor"),
	Shield UMETA(DisplayName = "Shield")
};


UENUM(BlueprintType)
enum class ESmashCharacter : uint8
{
	None UMETA(DisplayName = "None"),
	Fighter UMETA(DisplayName = "Fighter"),
	SwordMan UMETA(DisplayName = "SwordMan"),
	Gunner UMETA(DisplayName = "Gunner"),
	Hats UMETA(DisplayName = "Hats"),
	Mage UMETA(DisplayName = "Mage"),
	Bob UMETA(DisplayName = "Bob"),
	Random UMETA(DisplayName = "Random")
};

UENUM(BlueprintType)
enum class ESmashPlayerStates : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	WalkAndRun UMETA(DisplayName = "Walk/Run"),
	Crouch UMETA(DisplayName = "Crouch"),
	Jump UMETA(DisplayName = "Jump"),
	Fall UMETA(DisplayName = "Fall"),
	Hit UMETA(DisplayName = "Hit"),
	Launch UMETA(DisplayName = "Launch"),
	Ledge UMETA(DisplayName = "Ledge"),
	Dead UMETA(DisplayName = "Dead"),
	Tumble UMETA(DisplayName = "Tumble"),
	ShieldTumble UMETA(DisplayName = "Shield Tumble"),
	Prone UMETA(DisplayName = "Prone"),
	Stun UMETA(DisplayName = "Stun"),
	Dizzy UMETA(DisplayName = "Dizzy"),
	Ability UMETA(DisplayName = "Ability"),
	Dodge UMETA(DisplayName = "Dodge"),
	FreeFall UMETA(DisplayName = "Free Fall"),
	Held UMETA(DisplayName = "Held"),
	Hold UMETA(DisplayName = "Hold"),
	Down UMETA(DisplayName = "Down")
};

UENUM(BlueprintType)
enum class ESmashAttacks : uint8
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
enum class ESmashAbilityTypes : uint8
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

USTRUCT(BlueprintType)
struct FSmashPlayerMovement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	bool bCanAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	bool bCanJump = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	bool bCanFlipping = true;

	FSmashPlayerMovement() : bCanAttack(false), bCanMove(true), bCanJump(true), bCanFlipping(true)
	{
	}

	FSmashPlayerMovement(bool InCanAttack, bool InCanMove, bool InCanJump, bool InCanFlipping) : bCanAttack(InCanAttack), bCanMove(InCanMove), bCanJump(InCanJump), bCanFlipping(InCanFlipping)
	{
	}
};

USTRUCT(BlueprintType)
struct FSmashPlayCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Condition")
	bool bCanSmash = false;
};

USTRUCT(BlueprintType)
struct FSmashPlayerStateInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	FSmashPlayerMovement PlayerMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Info")
	FSmashPlayCondition PlayCondition;
};
