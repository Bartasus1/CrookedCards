// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KrzyweKarty2 : ModuleRules
{
	public KrzyweKarty2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "StructUtils" });

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"UMG", 
			"Slate", 
			"SlateCore", 
			"RenderCore", 
			"DeveloperSettings"
		});

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {"SubobjectDataInterface" });
		}
		
		//PrivateDependencyModuleNames.Add("OnlineSubsystemSteam");
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		
	}
}
