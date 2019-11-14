// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CameraShakeEditor.h"

#include "AssetData.h"
#include "Misc/MessageDialog.h"
#include "HAL/FileManager.h"
#include "Misc/ScopedSlowTask.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/SlateApplication.h"
#include "EditorStyleSet.h"
#include "Editor/UnrealEdEngine.h"
#include "Editor.h"
#include "UnrealEdGlobals.h"

#include "Camera/CameraShake.h"

#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "IDetailCustomization.h"

#include "BusyCursor.h"
#include "Editor/UnrealEd/Private/GeomFitUtils.h"
#include "EditorViewportCommands.h"
#include "Editor/UnrealEd/Private/ConvexDecompTool.h"

#include "Widgets/Docking/SDockTab.h"
#include "Framework/Commands/GenericCommands.h"
#include "Widgets/Input/STextComboBox.h"

#include "SCameraShakeEditorViewport.h"
#include "CameraShakeEditorModule.h"
#include "CameraShakeEditorActions.h"


#define LOCTEXT_NAMESPACE "CameraShakeEditor"


const FName FCameraShakeEditor::ViewportTabId( TEXT( "CameraShakeEditor_Viewport" ) );

const FName CameraShakeEditorAppIdentifier = FName(TEXT("CameraShakeEditorApp"));


void FCameraShakeEditor::InitEditorForCameraShake(UCameraShake* InObjectToEdit)
{
    // Support undo/redo
    InObjectToEdit->SetFlags(RF_Transactional);

    GEditor->RegisterForUndo(this);

    // Register our commands. This will only register them if not previously registered
    FCameraShakeEditorCommands::Register();

    BindCommands();

    Viewport = SNew(SCameraShakeEditorViewport)
        .CameraShakeEditor(SharedThis(this))
        .ObjectToEdit(InObjectToEdit);


    SetCameraShake(InObjectToEdit);
}


void FCameraShakeEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_CameraShakeEditor", "Camera Shake Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(
        ViewportTabId, FOnSpawnTab::CreateSP(this, &FCameraShakeEditor::SpawnTab_Viewport) 
    )
    .SetDisplayName(LOCTEXT("ViewportTab", "Viewport"))
    .SetGroup(WorkspaceMenuCategoryRef)
    .SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	OnRegisterTabSpawners().Broadcast(InTabManager);
}

void FCameraShakeEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner( ViewportTabId );

	OnUnregisterTabSpawners().Broadcast(InTabManager);
}


FCameraShakeEditor::~FCameraShakeEditor()
{
	OnCameraShakeEditorClosed().Broadcast();

	GEditor->UnregisterForUndo( this );
}

void FCameraShakeEditor::InitCameraShakeEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UCameraShake* InObjectToEdit)
{
    if (InObjectToEdit != CameraShake)
    {
        check(false);
        InitEditorForCameraShake(InObjectToEdit);
    }

    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CameraShakeEditor_Layout_v1.0")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.1f)
                ->SetHideTabWell(true)
                ->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
                ->SetSizeCoefficient(0.9f)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.6f)
                    ->AddTab(ViewportTabId, ETabState::OpenedTab)
                    ->SetHideTabWell(true)
                )
            )
        );

    const bool bCreateDefaultStandaloneMenu = true;
    const bool bCreateDefaultToolbar = true;
    FAssetEditorToolkit::InitAssetEditor(
        Mode,
        InitToolkitHost,
        CameraShakeEditorAppIdentifier,
        StandaloneDefaultLayout,
        bCreateDefaultToolbar, bCreateDefaultStandaloneMenu,
        InObjectToEdit
    );

    ExtendMenu();
    RegenerateMenusAndToolbars();
}

void FCameraShakeEditor::ExtendMenu()
{

}

void FCameraShakeEditor::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( CameraShake );
}

TSharedRef<SDockTab> FCameraShakeEditor::SpawnTab_Viewport( const FSpawnTabArgs& Args )
{
	check( Args.GetTabId() == ViewportTabId );

	TSharedRef<SDockTab> SpawnedTab =
	 SNew(SDockTab)
		.Label( LOCTEXT("CameraShakeViewport_TabTitle", "Viewport") )
		[
			Viewport.ToSharedRef()
		];

	Viewport->SetParentTab( SpawnedTab );

	return SpawnedTab;
}


void FCameraShakeEditor::BindCommands()
{

}


FName FCameraShakeEditor::GetToolkitFName() const
{
	return FName("CameraShakeEditor");
}

FText FCameraShakeEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "CameraShake Editor");
}

FString FCameraShakeEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "CameraShake ").ToString();
}

FLinearColor FCameraShakeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor( 0.3f, 0.2f, 0.5f, 0.5f );
}

void FCameraShakeEditor::RefreshTool()
{
	RefreshViewport();
}

void FCameraShakeEditor::RefreshViewport()
{
	Viewport->RefreshViewport();
}

FEditorViewportClient& FCameraShakeEditor::GetViewportClient()
{
    return Viewport->GetViewportClient();
}

void FCameraShakeEditor::SetCameraShake(UCameraShake* InCameraShake, bool bResetCamera)
{
    CameraShake = InCameraShake;

	Viewport->UpdateCameraShake(CameraShake);
	Viewport->RefreshViewport();
}

EViewModeIndex FCameraShakeEditor::GetViewMode() const
{
	if (Viewport.IsValid())
	{
		const FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();
		return ViewportClient.GetViewMode();
	}
	else
	{
		return VMI_Unknown;
	}
}

void FCameraShakeEditor::UndoAction()
{
	GEditor->UndoTransaction();
}

void FCameraShakeEditor::RedoAction()
{
	GEditor->RedoTransaction();
}

void FCameraShakeEditor::Tick(float DeltaTime)
{

}

TStatId FCameraShakeEditor::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FCameraShakeEditor, STATGROUP_TaskGraphTasks);
}

#undef LOCTEXT_NAMESPACE
