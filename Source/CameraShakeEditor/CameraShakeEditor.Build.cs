// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CameraShakeEditor : ModuleRules
{
	public CameraShakeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        {
            bEnableShadowVariableWarnings = false;

            bEnforceIWYU = true;
            bFasterWithoutUnity = true;

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        }

        PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetRegistry",
				"Kismet",
				"EditorWidgets",
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
                "AdvancedPreviewScene",
                "AssetTools"
            }
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"SceneOutliner",
				"ClassViewer",
				"ContentBrowser",
				"WorkspaceMenuStructure",
            }
		);
	}
}
