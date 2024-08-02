using UnrealBuildTool;

public class KrzyweKarty2Editor : ModuleRules
{
    public KrzyweKarty2Editor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "AssetTools",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "InputCore",
                "Engine",
                "KrzyweKarty2",
                "SlateCore",
                "Slate",
                "UnrealEd",
                "BlueprintGraph",
                "GraphEditor",
                "PropertyEditor",
                "EditorStyle",
                "Kismet",
                "KismetCompiler"
            }
        );
    }
}