// 


#include "SmashMacroLibrary.h"

void USmashMacroLibrary::ElseBranch(bool Con1, bool Con2, bool Con3, bool Con4, EElseBranchResult& BranchResult)
{
	if (Con1)
	{
		BranchResult = EElseBranchResult::IfCon1;
	}
	else if (Con2)
	{
		BranchResult = EElseBranchResult::ElseIfCon2;
	}
	else if (Con3)
	{
		BranchResult = EElseBranchResult::ElseIfCon3;
	}
	else if (Con4)
	{
		BranchResult = EElseBranchResult::ElseIfCon4;
	}
	else
	{
		BranchResult = EElseBranchResult::Else;
	}
}