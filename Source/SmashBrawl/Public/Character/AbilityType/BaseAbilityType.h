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
	UBaseAbilityType() : AbilityType(EAbilityTypes::None) {};
	UBaseAbilityType(EAbilityTypes AbilityTypes) : AbilityType(AbilityTypes) {};

	EAbilityTypes GetAbilityType() const;

	virtual void EnterState() override;
	virtual void ExitState() override;
protected:
	EAbilityTypes AbilityType;
};
UCLASS()
class UNoneAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UNoneAbilityType() : UBaseAbilityType(EAbilityTypes::None) {};
};
UCLASS()
class UBasicAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UBasicAbilityType() : UBaseAbilityType(EAbilityTypes::Basic) {};
	
};
UCLASS()
class USpecialAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	USpecialAbilityType() : UBaseAbilityType(EAbilityTypes::Special) {};
};
UCLASS()
class UAirAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UAirAbilityType() : UBaseAbilityType(EAbilityTypes::Air) {};
};
UCLASS()
class USmashAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:

	USmashAbilityType() : UBaseAbilityType(EAbilityTypes::Smash) {};
};
UCLASS()
class USuperAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	USuperAbilityType() : UBaseAbilityType(EAbilityTypes::Super) {};
};
UCLASS()
class UDodgeAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UDodgeAbilityType() : UBaseAbilityType(EAbilityTypes::Dodge) {};
};
UCLASS()
class UTauntAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UTauntAbilityType() : UBaseAbilityType(EAbilityTypes::Taunt) {};
};
UCLASS()
class ULedgeAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	ULedgeAbilityType() : UBaseAbilityType(EAbilityTypes::Ledge) {};
};
UCLASS()
class UThrowAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UThrowAbilityType() : UBaseAbilityType(EAbilityTypes::Throw) {};
};
UCLASS()
class UProneAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UProneAbilityType() : UBaseAbilityType(EAbilityTypes::Prone) {};
};
UCLASS()
class UGrabAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UGrabAbilityType() : UBaseAbilityType(EAbilityTypes::Grab) {};
};
UCLASS()
class UOtherAbilityType : public UBaseAbilityType
{
	GENERATED_BODY()

public:
	UOtherAbilityType() : UBaseAbilityType(EAbilityTypes::Other) {};
};