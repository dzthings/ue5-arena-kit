using UnrealBuildTool;

public class ArenaKitEditorTarget : TargetRules
{
	public ArenaKitEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("ArenaKit");
	}
}
