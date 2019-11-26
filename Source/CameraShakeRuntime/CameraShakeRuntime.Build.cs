// Copyright Peter Leontev, peterleontev.com

using UnrealBuildTool;

public class CameraShakeRuntime : ModuleRules
{
	public CameraShakeRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        {
            bEnableShadowVariableWarnings = false;

            bEnforceIWYU = true;
            bFasterWithoutUnity = true;

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        }

        PrivateIncludePathModuleNames.AddRange(
			new string[] {
                "Kismet"
            }
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"Engine"
            }
		);
	}
}
