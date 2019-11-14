// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_CameraShake.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EditorStyleSet.h"

#include "CameraShakeEditorModule.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"


void FAssetTypeActions_CameraShake::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor )
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto CameraShake = Cast<UCameraShake>(*ObjIt);
		if (CameraShake != nullptr)
		{
			ICameraShakeEditorModule* CameraShakeEditorModule = &FModuleManager::LoadModuleChecked<ICameraShakeEditorModule>( "CameraShakeEditor" );
            CameraShakeEditorModule->CreateCameraShakeEditor(Mode, EditWithinLevelEditor, CameraShake);
		}
	}
}

#undef LOCTEXT_NAMESPACE
