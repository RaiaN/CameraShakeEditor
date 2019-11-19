// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CameraShakeEditorActions.h"

#define LOCTEXT_NAMESPACE "CameraShakeEditorCommands"

void FCameraShakeEditorCommands::RegisterCommands()
{
    UI_COMMAND(ResetCamera, "Reset Camera", "Resets the camera to default settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
