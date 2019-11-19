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
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraTypes.h"

#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "IDetailCustomization.h"

#include "EditorViewportCommands.h"

#include "Widgets/Docking/SDockTab.h"
#include "Framework/Commands/GenericCommands.h"
#include "Widgets/Input/STextComboBox.h"

#include "SCameraShakeEditorViewport.h"
#include "CameraShakeEditorModule.h"
#include "CameraShakeEditorActions.h"
#include "CameraShakeDetails.h"


#define LOCTEXT_NAMESPACE "CameraShakeEditor"


const FName FCameraShakeEditor::ViewportTabId(TEXT("CameraShakeEditor_Viewport"));
const FName FCameraShakeEditor::PropertiesTabId(TEXT("CameraShakeEditor_Properties"));

const FName CameraShakeEditorAppIdentifier = FName(TEXT("CameraShakeEditorApp"));


void FCameraShakeEditor::InitEditorForCameraShake(UCameraShake* InObjectToEdit)
{
    // Support undo/redo
    InObjectToEdit->SetFlags(RF_Transactional);

    GEditor->RegisterForUndo(this);

    // Register our commands. This will only register them if not previously registered
    FCameraShakeEditorCommands::Register();

    Viewport = SNew(SCameraShakeEditorViewport)
        .CameraShakeEditor(SharedThis(this))
        .ObjectToEdit(InObjectToEdit);

    BindCommands();

    ExtendToolbar();


    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

        FDetailsViewArgs DetailsViewArgs;
        DetailsViewArgs.bAllowSearch = true;
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.bUpdatesFromSelection = false;
        DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
        DetailsViewArgs.NotifyHook = this;

        CameraShakeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

        FOnGetDetailCustomizationInstance LayoutCustomCameraShakeProperties = FOnGetDetailCustomizationInstance::CreateSP(this, &FCameraShakeEditor::MakeCameraShakeDetails);
        CameraShakeDetailsView->RegisterInstancedCustomPropertyLayout(UCameraShake::StaticClass(), LayoutCustomCameraShakeProperties);
    }



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

    InTabManager->RegisterTabSpawner(
        PropertiesTabId, FOnSpawnTab::CreateSP(this, &FCameraShakeEditor::SpawnTab_Properties)
    )
        .SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	OnRegisterTabSpawners().Broadcast(InTabManager);
}

void FCameraShakeEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

    InTabManager->UnregisterTabSpawner(ViewportTabId);
    InTabManager->UnregisterTabSpawner(PropertiesTabId);

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
                ->Split
                (
                    FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
                    ->SetSizeCoefficient(0.2f)
                    ->Split
                    (
                        FTabManager::NewStack()
                        ->SetSizeCoefficient(0.7f)
                        ->AddTab(PropertiesTabId, ETabState::OpenedTab)
                    )
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

TSharedRef<class IDetailCustomization> FCameraShakeEditor::MakeCameraShakeDetails()
{
    TSharedRef<FCameraShakeDetails> NewDetails = MakeShareable(new FCameraShakeDetails(*this));
    CameraShakeDetails = NewDetails;
    return NewDetails;
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

TSharedRef<SDockTab> FCameraShakeEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == PropertiesTabId);

    return SNew(SDockTab)
        .Icon(FEditorStyle::GetBrush("CameraShakeEditor.Tabs.Properties"))
        .Label(LOCTEXT("CameraShakeProperties_TabTitle", "Details"))
        [
            CameraShakeDetailsView.ToSharedRef()
        ];
}


void FCameraShakeEditor::BindCommands()
{
    const FCameraShakeEditorCommands& Commands = FCameraShakeEditorCommands::Get();

    const TSharedRef<FUICommandList>& UICommandList = GetToolkitCommands();

    UICommandList->MapAction(
        Commands.ResetCamera,
        FExecuteAction::CreateSP(this, &FCameraShakeEditor::ResetCamera)
    );
}


void FCameraShakeEditor::ExtendToolbar()
{
    struct Local
    {
        static void FillToolbar(FToolBarBuilder& ToolbarBuilder, FCameraShakeEditor* ThisEditor)
        {
            ToolbarBuilder.BeginSection("Camera");
            {
                ToolbarBuilder.AddToolBarButton(FCameraShakeEditorCommands::Get().ResetCamera);
            }
            ToolbarBuilder.EndSection();
        }
    };

    TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

    FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();

    FCameraShakeEditor* ThisEditor = this;

    ToolbarExtender->AddToolBarExtension(
        "Asset",
        EExtensionHook::After,
        Viewport->GetCommandList(),
        FToolBarExtensionDelegate::CreateStatic(&Local::FillToolbar, ThisEditor)
    );

    AddToolbarExtender(ToolbarExtender);
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
    bool bForceRefresh = true;
    CameraShakeDetailsView->SetObject(CameraShake, bForceRefresh);

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

    // Set the details view.
    CameraShakeDetailsView->SetObject(CameraShake);

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

void FCameraShakeEditor::ResetCamera()
{
    FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();

    ViewportClient.SetViewLocation(FVector::ZeroVector);
    ViewportClient.SetViewRotation(FRotator::ZeroRotator);
    ViewportClient.ViewFOV = EditorViewportDefs::DefaultPerspectiveFOVAngle;
}

void FCameraShakeEditor::Tick(float DeltaTime)
{
    if (CameraShake->OscillatorTimeRemaining > 0.0f)
    {
        FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();

        FMinimalViewInfo MinimalViewInfo;
        MinimalViewInfo.Location = ViewportClient.GetViewLocation();
        MinimalViewInfo.Rotation = ViewportClient.GetViewRotation();
        MinimalViewInfo.FOV = ViewportClient.ViewFOV;

        {
            CameraShake->UpdateAndApplyCameraShake(DeltaTime, 1.0f, MinimalViewInfo);
        }

        ViewportClient.SetViewLocation(MinimalViewInfo.Location);
        ViewportClient.SetViewRotation(MinimalViewInfo.Rotation);
        ViewportClient.ViewFOV = MinimalViewInfo.FOV;
    }    
}

TStatId FCameraShakeEditor::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FCameraShakeEditor, STATGROUP_TaskGraphTasks);
}

FReply FCameraShakeEditor::PlayCameraShake()
{
    CameraShake->OscillatorTimeRemaining = CameraShake->OscillationBlendOutTime + 1.0f;

    CameraShake->PlayShake(nullptr, 1.0f, ECameraAnimPlaySpace::World);

    return FReply::Handled();
}

FReply FCameraShakeEditor::StopCameraShake()
{
    bool bImmediately = true;
    CameraShake->StopShake(bImmediately);

    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
