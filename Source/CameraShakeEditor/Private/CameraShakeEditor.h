// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "Misc/NotifyHook.h"
#include "ICameraShakeEditor.h"
#include "Input/Reply.h"
#include "TickableEditorObject.h"


class FEditorViewportClient;
class IDetailsView;
class SAdvancedPreviewDetailsTab;
class SDockableTab;
class SCameraShakeEditorViewport;
struct FPropertyChangedEvent;
struct FTabSpawnerEntry;
class UCameraShake;

/**
 * CameraShake Editor class
 */
class FCameraShakeEditor : public ICameraShakeEditor, public FGCObject, public FEditorUndoClient, public FNotifyHook, public FTickableEditorObject
{
public:
    FCameraShakeEditor() : CameraShake(nullptr)
	{}

	~FCameraShakeEditor();

	/** Initializes the editor to use a camera shake. Should be the first thing called. */
	void InitEditorForCameraShake(UCameraShake* InCameraShake);

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	/**
	 * Edits the specified camera shake object
	 *
	 * @param	Mode					Asset editing mode for this editor (standalone or world-centric)
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	ObjectToEdit			The camera shake to edit
	 */
	void InitCameraShakeEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UCameraShake* InObjectToEdit);

    /** Creates details for a static mesh */
    TSharedRef<class IDetailCustomization> MakeCameraShakeDetails();

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


private:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);

private:
	/** Binds commands associated with the Camera Shake Editor. */
	void BindCommands();

	
	void SetCameraShake(UCameraShake* InCameraShake, bool bResetCamera = true);

	/** Undo Action**/
	void UndoAction();

	/** Redo Action **/
	void RedoAction();

private:
	/** List of open tool panels; used to ensure only one exists at any one time */
	TMap< FName, TWeakPtr<class SDockableTab> > SpawnedToolPanels;

	/** Preview Viewport widget */
	TSharedPtr<class SCameraShakeEditorViewport> Viewport;

    /** Property View */
    TSharedPtr<class IDetailsView> CameraShakeDetailsView;

    /** Static mesh editor detail customization */
    TWeakPtr<class FCameraShakeDetails> CameraShakeDetails;

	/** The currently active Camera Shake. */
	UCameraShake* CameraShake;

	/**	The tab ids for all the tabs used */
	static const FName ViewportTabId;
    static const FName PropertiesTabId;

	/** A local reference to the private Extender set in FAssetEditorToolkit so that we can extend our Toolbar sub-menus */
	TSharedPtr<FExtender> EditorToolbarExtender;
};
