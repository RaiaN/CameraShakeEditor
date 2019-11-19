// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.



#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

/**
 * Unreal CameraShake editor actions
 */
class FCameraShakeEditorCommands : public TCommands<FCameraShakeEditorCommands>
{

public:
	FCameraShakeEditorCommands() : TCommands<FCameraShakeEditorCommands>
	(
		"CameraShakeEditor", // Context name for fast lookup
		NSLOCTEXT("Contexts", "CameraShakeEditor", "CameraShake Editor"), // Localized context name for displaying
		"EditorViewport",  // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
	{
	}

	/**
	 * Initialize commands
	 */
	virtual void RegisterCommands() override;

public:
    TSharedPtr< FUICommandInfo > ResetCamera;
};
