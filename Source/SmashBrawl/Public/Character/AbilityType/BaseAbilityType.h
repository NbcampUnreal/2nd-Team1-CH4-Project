// 

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypeInterface.h"
#include "Core/SmashTypes.h"
#include "UObject/Object.h"
#include "BaseAbilityType.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API UBaseAbilityType : public UObject, public IInterface_AbilityTypeInterface
{
	GENERATED_BODY()

public:
	UBaseAbilityType() : AbilityType(ESmashAbilityTypes::None) {};
	UBaseAbilityType(ESmashAbilityTypes AbilityTypes) : AbilityType(AbilityTypes) {};

	ESmashAbilityTypes GetAbilityType() const;

	virtual void EnterState() override;
	virtual void ExitState() override;
protected:
	ESmashAbilityTypes AbilityType;
};
UCLASS()
class UNoneAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UNoneAbilityType() : UBaseAbilityType(ESmashAbilityTypes::None) {};
};
UCLASS()
class UBasicAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UBasicAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Basic) {};
	
};
UCLASS()
class USpecialAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	USpecialAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Special) {};
};
UCLASS()
class UAirAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UAirAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Air) {};
};
UCLASS()
class USmashAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:

	USmashAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Smash) {};
};
UCLASS()
class USuperAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	USuperAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Super) {};
};
UCLASS()
class UDodgeAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UDodgeAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Dodge) {};
};
UCLASS()
class UTauntAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UTauntAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Taunt) {};
};
UCLASS()
class ULedgeAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	ULedgeAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Ledge) {};
};
UCLASS()
class UThrowAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UThrowAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Throw) {};
};
UCLASS()
class UProneAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UProneAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Prone) {};
};
UCLASS()
class UGrabAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UGrabAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Grab) {};
};
UCLASS()
class UOtherAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UOtherAbilityType() : UBaseAbilityType(ESmashAbilityTypes::Other) {};
};