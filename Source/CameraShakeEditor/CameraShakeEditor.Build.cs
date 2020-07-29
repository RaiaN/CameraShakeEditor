// Copyright 2020 Peter Leontev. All Rights Reserved.

using UnrealBuildTool;

public class CameraShakeEditor : ModuleRules
{
	public CameraShakeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        {
            bEnforceIWYU = true;
            bUseUnity = false;

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        }

        PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetRegistry",
				"Kismet",
                "PropertyEditor",
                "AssetTools"
            }
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"Engine",
                "InputCore",
				"Slate",
				"SlateCore",
                "EditorStyle",
				"UnrealEd",
                "PropertyEditor",
                "AssetTools",
                "Projects",
                "CameraShakeRuntime"
            }
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"ClassViewer"
            }
		);
	}
}
