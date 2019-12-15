// Copyright 2019 Peter Leontev. All Rights Reserved.

#include "CameraShakeEditorModule.h"

#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Brushes/SlateImageBrush.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

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
        const FString& PluginContent = IPluginManager::Get().FindPlugin("CameraShakeEditor")->GetBaseDir() + "/Content";

        CameraShakeStyleSet = MakeShareable(new FSlateStyleSet("CameraShake"));
        CameraShakeStyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
        CameraShakeStyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

        const FVector2D Vec16(16.0f, 16.0f);
        const FVector2D Vec64(64.0f, 64.0f);

        CameraShakeStyleSet->Set("ClassIcon.CameraShake", new FSlateImageBrush(PluginContent + "/CameraShakeAsset_16.png", Vec16));
        CameraShakeStyleSet->Set("ClassThumbnail.CameraShake", new FSlateImageBrush(PluginContent + "/CameraShakeAsset_64.png", Vec64));

        FSlateStyleRegistry::RegisterSlateStyle(*CameraShakeStyleSet.Get());




        MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);

        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_CameraShake));
    }

    /**
     * Called before the module is unloaded, right before the module object is destroyed.
     */
    virtual void ShutdownModule() override
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*CameraShakeStyleSet.Get());

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

    TSharedPtr<FSlateStyleSet> CameraShakeStyleSet;
};



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCameraShakeEditorModule, CameraShakeEditor)