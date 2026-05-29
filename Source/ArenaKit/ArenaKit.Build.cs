using UnrealBuildTool;

public class ArenaKit : ModuleRules
{
	public ArenaKit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"EnhancedInput", "AIModule", "NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
