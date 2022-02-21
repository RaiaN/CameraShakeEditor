// Copyright 2020 Peter Leontev. All Rights Reserved.

using UnrealBuildTool;

public class CameraShakeRuntime : ModuleRules
{
	public CameraShakeRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        {
            bEnforceIWYU = true;
            bUseUnity = false;

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
				"Engine",
                "GameplayCameras"
            }
		);
	}
}
