// Copyright 2020 Peter Leontev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Toolkits/AssetEditorToolkit.h"


class UCameraShake;

/**
 * Public interface to Camera Shake Editor
 */
class ICameraShakeEditor : public FAssetEditorToolkit
{
public:
     /** Refreshes the Camera Shake Editor's viewport. */
    virtual void RefreshViewport() = 0;

    /** Refreshes everything in the Camera Shake Editor. */
    virtual void RefreshTool() = 0;

    /** Get the active view mode */
    virtual EViewModeIndex GetViewMode() const = 0;

    /** Get the Camera Shake Editor's the viewport client */
    virtual class FEditorViewportClient& GetViewportClient() = 0;

    DECLARE_EVENT_OneParam(ICameraShakeEditor, FCameraShakeEditorDockingExtentionTabsEvent, const TSharedRef<FTabManager::FStack>&)
    virtual FCameraShakeEditorDockingExtentionTabsEvent& OnCameraShakeEditorDockingExtentionTabs() { return CameraShakeEditorDockingExtentionTabsEvent; };

    DECLARE_EVENT(ICameraShakeEditor, FCameraShakeEditorClosedEvent);
    virtual FCameraShakeEditorClosedEvent& OnCameraShakeEditorClosed() { return CameraShakeEditorClosedEvent; };

    /** Delegate to be called when the tabs are being registered **/
    DECLARE_EVENT_OneParam(ICameraShakeEditor, FRegisterTabSpawnersEvent, const TSharedRef<class FTabManager>&);
    virtual FRegisterTabSpawnersEvent& OnRegisterTabSpawners() { return RegisterTabSpawnersEvent; };

    /** Delegate to be called when the tabs are being unregistered **/
    DECLARE_EVENT_OneParam(ICameraShakeEditor, FUnregisterTabSpawnersEvent, const TSharedRef<class FTabManager>&);
    virtual FUnregisterTabSpawnersEvent& OnUnregisterTabSpawners() { return UnregisterTabSpawnersEvent; };

private:
    FCameraShakeEditorDockingExtentionTabsEvent CameraShakeEditorDockingExtentionTabsEvent;
    FCameraShakeEditorClosedEvent CameraShakeEditorClosedEvent;
    FRegisterTabSpawnersEvent RegisterTabSpawnersEvent;
    FUnregisterTabSpawnersEvent UnregisterTabSpawnersEvent;
};


