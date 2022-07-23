// Copyright 2020 Peter Leontev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "Misc/NotifyHook.h"
#include "ICameraShakeEditor.h"
#include "Input/Reply.h"
#include "TickableEditorObject.h"
#include "EditorUndoClient.h"


class FEditorViewportClient;
class IDetailsView;
class SAdvancedPreviewDetailsTab;
class SDockableTab;
class SCameraShakeEditorViewport;
struct FPropertyChangedEvent;
struct FTabSpawnerEntry;
class UCameraShakeBase;
class UCameraShakePlayParams;

/**
 * CameraShake Editor class
 */
class FCameraShakeEditor : public ICameraShakeEditor, public FGCObject, public FEditorUndoClient, public FNotifyHook, public FTickableEditorObject
{
public:
    FCameraShakeEditor() : CameraShake(nullptr), CameraShakeToPlay(nullptr), CameraShakeToPlayParams(nullptr)
	{}

	~FCameraShakeEditor();

	/** Initializes the editor to use a camera shake. Should be the first thing called. */
	void InitEditorForCameraShake(UCameraShakeBase* InCameraShake);

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	/**
	 * Edits the specified camera shake object
	 *
	 * @param	Mode					Asset editing mode for this editor (standalone or world-centric)
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	ObjectToEdit			The camera shake to edit
	 */
	void InitCameraShakeEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UCameraShakeBase* InObjectToEdit);

    /** Creates general settings details for a camera shake */
    TSharedRef<class IDetailCustomization> MakeCameraShakeGeneralSettingsDetails();

    /** Creates play params details for a camera shake */
    TSharedRef<class IDetailCustomization> MakeCameraShakePlayParamsDetails();

	//~ Begin FGCObject Interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~ End FGCObject Interface

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;

	/** @return Returns the color and opacity to use for the color that appears behind the tab text for this toolkit's tab in world-centric mode. */
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void RefreshTool() override;
	virtual void RefreshViewport() override;;

	virtual FEditorViewportClient& GetViewportClient() override;
	// End of ICameraShakeEditor

	/** Extends the toolbar menu to include camera shake editor options */
	void ExtendMenu();

	/** Get the active view mode */
	virtual EViewModeIndex GetViewMode() const override;

	/** For asynchronous convex decomposition support, this class is tickable in the editor to be able to confirm
	that the process is completed */
	virtual bool IsTickableInEditor() const final
	{
		return true;
	}

	/** This is a tickable class */
	virtual ETickableTickType GetTickableTickType() const final
	{
		return ETickableTickType::Always;
	}

	/** Performs the main 'tick' operation on this class.  The 'tick' step checks to see if there is currently
	an active asynchronous convex decomopsition task running and, if so, checks to see if it is completed and,
	if so, gathers the results and releases the interface */
	virtual void Tick(float DeltaTime) final;

	/** Returns the stat ID for this tickable class */
	virtual TStatId GetStatId() const final;

public:
    FReply PlayCameraShake();
    FReply StopCameraShake();
    FReply ResetCamera();


private:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_PlayParams(const FSpawnTabArgs& Args);

private:
	/** Binds commands associated with the Camera Shake Editor. */
	void BindCommands();

    void ExtendToolbar();

	
	void SetCameraShake(UCameraShakeBase* InCameraShake, bool bResetCamera = true);

	void UndoAction();

	void RedoAction();

private:
	/** List of open tool panels; used to ensure only one exists at any one time */
	TMap< FName, TWeakPtr<class SDockableTab> > SpawnedToolPanels;

	/** Preview Viewport widget */
	TSharedPtr<class SCameraShakeEditorViewport> Viewport;

private:
    /** Property View */
    TSharedPtr<class IDetailsView> CameraShakeDetailsView;

    /** Camera shake editor general settings */
    TWeakPtr<class FCameraShakeDetails> CameraShakeDetails;

private:
    /** Property View */
    TSharedPtr<class IDetailsView> CameraShakePlayParamsView;

    /** Camera shake editor play params */
    TWeakPtr<class FCameraShakePlayParamsDetails> CameraShakePlayParamsDetails;

private:
	/** The currently active Camera Shake. */
	UCameraShakeBase* CameraShake;

    /** The Camera Shake to use for debugging. */
    UCameraShakeBase* CameraShakeToPlay;

    UCameraShakePlayParams* CameraShakeToPlayParams;

	/**	The tab ids for all the tabs used */
	static const FName ViewportTabId;
    static const FName PropertiesTabId;
    static const FName PlayParamsTabId;
};
