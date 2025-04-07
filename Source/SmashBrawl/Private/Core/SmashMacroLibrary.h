// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SmashMacroLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EElseBranchResult : uint8
{
	IfCon1      UMETA(DisplayName="If Con 1"),
	ElseIfCon2  UMETA(DisplayName="Else If Con 2"),
	ElseIfCon3  UMETA(DisplayName="Else If Con 3"),
	ElseIfCon4  UMETA(DisplayName="Else If Con 4"),
	Else        UMETA(DisplayName="Else")
};

UENUM(BlueprintType)
enum class EAttackConditionResult : uint8
{
	NotPressed  UMETA(DisplayName="Not Pressed"),
	Pressed     UMETA(DisplayName="Pressed")
};


UCLASS()
class SMASHBRAWL_API USmashMacroLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Macros", meta=(ExpandEnumAsExecs="BranchResult"))
	static void ElseBranch(bool Con1, bool Con2, bool Con3, bool Con4, EElseBranchResult& BranchResult);
};