// Copyright Peter Leontev, peterleontev.com

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
};
