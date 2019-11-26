// Copyright Peter Leontev, peterleontev.com

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkit.h"
#include "ICameraShakeEditor.h"


class UCameraShake;
class FSlateStyleSet;


class ICameraShakeEditorModule : public IModuleInterface, public IHasMenuExtensibility
{
public:
    /**
     * Creates a new camera shake editor.
     */
    virtual TSharedRef<ICameraShakeEditor> CreateCameraShakeEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UCameraShake* CameraShake) = 0;

    /** Delegate to be called when a Material Editor is created, for toolbar, tab, and menu extension **/
    DECLARE_EVENT_OneParam(ICameraShakeEditorModule, FCameraShakeEditorOpenedEvent, TWeakPtr<ICameraShakeEditor>);
    virtual FCameraShakeEditorOpenedEvent& OnCameraShakeEditorOpened() { return CameraShakeEditorOpenedEvent; };

private:
    FCameraShakeEditorOpenedEvent CameraShakeEditorOpenedEvent;
};
