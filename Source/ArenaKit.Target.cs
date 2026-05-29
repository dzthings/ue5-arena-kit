using UnrealBuildTool;

public class ArenaKitTarget : TargetRules
{
	public ArenaKitTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("ArenaKit");
	}
}
