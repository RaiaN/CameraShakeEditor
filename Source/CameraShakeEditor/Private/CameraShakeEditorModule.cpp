// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CameraShakeEditorModule.h"

#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"

#include "AssetTypeActions_CameraShake.h"
#include "CameraShakeEditor.h"
#include "ICameraShakeEditor.h"



#define LOCTEXT_NAMESPACE "FCameraShakeEditorModule"


/**
 * CameraShake editor module
 */
class FCameraShakeEditorModule : public ICameraShakeEditorModule
{
public:
    /** Constructor, set up console commands and variables **/
    FCameraShakeEditorModule()
    {
    }

    /**
     * Called right after the module DLL has been loaded and the module object has been created
     */
    virtual void StartupModule() override
    {
        // Make sure the advanced preview scene module is loaded
        FModuleManager::Get().LoadModuleChecked("AdvancedPreviewScene");

        MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);

        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_CameraShake));
    }

    /**
     * Called before the module is unloaded, right before the module object is destroyed.
     */
    virtual void ShutdownModule() override
    {
        MenuExtensibilityManager.Reset();
    }

    /**
     * Creates a new CameraShake editor for a CameraShake
     */
    virtual TSharedRef<ICameraShakeEditor> CreateCameraShakeEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UCameraShake* CameraShake)
    {
        TSharedRef<FCameraShakeEditor> NewCameraShakeEditor(new FCameraShakeEditor());
        NewCameraShakeEditor->InitEditorForCameraShake(CameraShake);
        OnCameraShakeEditorOpened().Broadcast(NewCameraShakeEditor);
        NewCameraShakeEditor->InitCameraShakeEditor(Mode, InitToolkitHost, CameraShake);
        return NewCameraShakeEditor;
    }

    /** Gets the extensibility managers for outside entities to extend camera shake editor's menus and toolbars */
    virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }

private:
    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
};



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCameraShakeEditorModule, CameraShakeEditor)