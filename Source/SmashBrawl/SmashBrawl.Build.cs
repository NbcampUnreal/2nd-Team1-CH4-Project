// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmashBrawl : ModuleRules
{
	public SmashBrawl(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UMG", "Slate", "SlateCore",
			"SST", "Niagara", "GeometryCollectionEngine","OnlineSubsystem", "OnlineSubsystemUtils"
        });
        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.Add("UMGEditor");
        }
        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
