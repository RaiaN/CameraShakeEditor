// Copyright 2020 Peter Leontev. All Rights Reserved.

#include "CameraShakeEditor.h"

#include "AssetData.h"
#include "Misc/MessageDialog.h"
#include "HAL/FileManager.h"
#include "Misc/ScopedSlowTask.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Docking/TabManager.h"
#include "EditorStyleSet.h"
#include "Editor/UnrealEdEngine.h"
#include "Editor.h"
#include "LevelEditorViewport.h"
#include "UnrealEdGlobals.h"

#include "MatineeCameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraTypes.h"

#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "IDetailCustomization.h"

#include "EditorViewportCommands.h"

#include "Widgets/Docking/SDockTab.h"
#include "Framework/Commands/GenericCommands.h"
#include "Widgets/Input/STextComboBox.h"

#include "CameraShakeLibrary.h"

#include "SCameraShakeEditorViewport.h"
#include "CameraShakeEditorModule.h"
#include "CameraShakeEditorActions.h"
#include "CameraShakeDetails.h"
#include "CameraShakePlayParams.h"
#include "CameraShakePlayParamsDetails.h"




#define LOCTEXT_NAMESPACE "CameraShakeEditor"


const FName FCameraShakeEditor::ViewportTabId(TEXT("CameraShakeEditor_Viewport"));
const FName FCameraShakeEditor::PropertiesTabId(TEXT("CameraShakeEditor_Properties"));
const FName FCameraShakeEditor::PlayParamsTabId(TEXT("CameraShakeEditor_PlayParams"));

const FName CameraShakeEditorAppIdentifier = FName(TEXT("CameraShakeEditorApp"));


void FCameraShakeEditor::InitEditorForCameraShake(UCameraShakeBase* InObjectToEdit)
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

    
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    {
        FDetailsViewArgs DetailsViewArgs;
        DetailsViewArgs.bAllowSearch = true;
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.bUpdatesFromSelection = false;
        DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
        DetailsViewArgs.NotifyHook = this;

        CameraShakeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

        FOnGetDetailCustomizationInstance LayoutCustomCameraShakeProperties = FOnGetDetailCustomizationInstance::CreateSP(this, &FCameraShakeEditor::MakeCameraShakeGeneralSettingsDetails);
        CameraShakeDetailsView->RegisterInstancedCustomPropertyLayout(UCameraShakeBase::StaticClass(), LayoutCustomCameraShakeProperties);
    }
    {
        FDetailsViewArgs DetailsViewArgs;
        DetailsViewArgs.bAllowSearch = true;
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.bUpdatesFromSelection = false;
        DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
        DetailsViewArgs.NotifyHook = this;

        CameraShakePlayParamsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

        FOnGetDetailCustomizationInstance LayoutCustomCameraShakePlayParamsProperties = FOnGetDetailCustomizationInstance::CreateSP(this, &FCameraShakeEditor::MakeCameraShakePlayParamsDetails);
        CameraShakePlayParamsView->RegisterInstancedCustomPropertyLayout(UCameraShakePlayParams::StaticClass(), LayoutCustomCameraShakePlayParamsProperties);
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

    InTabManager->RegisterTabSpawner(
        PlayParamsTabId, FOnSpawnTab::CreateSP(this, &FCameraShakeEditor::SpawnTab_PlayParams)
    )
    .SetDisplayName(LOCTEXT("PlayParamsTab", "PlayParams"))
    .SetGroup(WorkspaceMenuCategoryRef)
    .SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.PlayParams"));

	OnRegisterTabSpawners().Broadcast(InTabManager);
}

void FCameraShakeEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

    InTabManager->UnregisterTabSpawner(ViewportTabId);
    InTabManager->UnregisterTabSpawner(PropertiesTabId);
    InTabManager->UnregisterTabSpawner(PlayParamsTabId);

    OnUnregisterTabSpawners().Broadcast(InTabManager);
}


FCameraShakeEditor::~FCameraShakeEditor()
{
	OnCameraShakeEditorClosed().Broadcast();

	GEditor->UnregisterForUndo( this );

    CameraShake = nullptr;
    CameraShakeToPlay = nullptr;
    CameraShakeToPlayParams = nullptr;
}

void FCameraShakeEditor::InitCameraShakeEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UCameraShakeBase* InObjectToEdit)
{
    if (InObjectToEdit != CameraShake)
    {
        check(false);
        InitEditorForCameraShake(InObjectToEdit);
    }

    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CameraShakeEditor_Layout_UE5_v1.1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Horizontal)
            ->SetSizeCoefficient(0.9f)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.8f)
                ->SetHideTabWell(true)
                ->AddTab(ViewportTabId, ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
                ->SetSizeCoefficient(0.2f)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetHideTabWell(true)
                    ->AddTab(PlayParamsTabId, ETabState::OpenedTab)

                )
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetHideTabWell(true)
                    ->AddTab(PropertiesTabId, ETabState::OpenedTab)
                )
            )
            ->AsArea().ToSharedRef()
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

TSharedRef<class IDetailCustomization> FCameraShakeEditor::MakeCameraShakeGeneralSettingsDetails()
{
    TSharedRef<FCameraShakeDetails> NewDetails = MakeShareable(new FCameraShakeDetails(*this));
    CameraShakeDetails = NewDetails;
    return NewDetails;
}

TSharedRef<class IDetailCustomization> FCameraShakeEditor::MakeCameraShakePlayParamsDetails()
{
    TSharedRef<FCameraShakePlayParamsDetails> NewDetails = MakeShareable(new FCameraShakePlayParamsDetails(*this));
    CameraShakePlayParamsDetails = NewDetails;
    return NewDetails;
}

void FCameraShakeEditor::ExtendMenu()
{

}

void FCameraShakeEditor::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( CameraShake );
    Collector.AddReferencedObject( CameraShakeToPlay );
    Collector.AddReferencedObject (CameraShakeToPlayParams );
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

    TSharedPtr<SDockTab> NewTab = SNew(SDockTab)
    [
        CameraShakeDetailsView.ToSharedRef()
    ];

    NewTab->SetTabIcon(FEditorStyle::GetBrush("CameraShakeEditor.Tabs.Properties"));
    NewTab->SetLabel(LOCTEXT("CameraShakeProperties_TabTitle", "Details"));

    return NewTab.ToSharedRef();
}

TSharedRef<SDockTab> FCameraShakeEditor::SpawnTab_PlayParams(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == PlayParamsTabId);

    TSharedPtr<SDockTab> NewTab = SNew(SDockTab)
    [
        CameraShakePlayParamsView.ToSharedRef()
    ];

    NewTab->SetTabIcon(FEditorStyle::GetBrush("CameraShakeEditor.Tabs.PlayParams"));
    NewTab->SetLabel(LOCTEXT("CameraShakePlayParams_TabTitle", "Play Params"));

    return NewTab.ToSharedRef();
}

void FCameraShakeEditor::BindCommands()
{
    
}


void FCameraShakeEditor::ExtendToolbar()
{
    struct Local
    {
        static void FillToolbar(FToolBarBuilder& ToolbarBuilder, FCameraShakeEditor* ThisEditor)
        {
            
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

void FCameraShakeEditor::SetCameraShake(UCameraShakeBase* InCameraShake, bool bResetCamera)
{
    CameraShake = InCameraShake;
    CameraShakeToPlayParams = NewObject<UCameraShakePlayParams>(CameraShake->GetOuter());
   
    CameraShakeDetailsView->SetObject(CameraShake);
    CameraShakePlayParamsView->SetObject(CameraShakeToPlayParams);

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
    if (IsValid(CameraShakeToPlay) && !CameraShakeToPlay->IsFinished())
    {
        FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();

        FMinimalViewInfo MinimalViewInfo;
        {
            MinimalViewInfo.Location = ViewportClient.GetViewLocation();
            MinimalViewInfo.Rotation = ViewportClient.GetViewRotation();
            MinimalViewInfo.FOV = ViewportClient.ViewFOV;
        }

        {
            UCameraShakeLibrary::CopyCameraShakeParams(CameraShake, CameraShakeToPlay);

            CameraShakeToPlay->UpdateAndApplyCameraShake(DeltaTime, 1.0f, MinimalViewInfo);
        }

        {
            ViewportClient.SetViewLocation(MinimalViewInfo.Location);
            ViewportClient.SetViewRotation(MinimalViewInfo.Rotation);
            ViewportClient.ViewFOV = MinimalViewInfo.FOV;
        }
    }    
}

TStatId FCameraShakeEditor::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FCameraShakeEditor, STATGROUP_TaskGraphTasks);
}

FReply FCameraShakeEditor::PlayCameraShake()
{
    FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();

    CameraShakeToPlay = NewObject<UMatineeCameraShake>(CameraShake->GetOuter(), UMatineeCameraShake::StaticClass());
    UCameraShakeLibrary::CopyCameraShakeParams(CameraShake, CameraShakeToPlay);

    CameraShakeToPlay->StartShake(
        nullptr,
        CameraShakeToPlayParams->ShakeScale,
        CameraShakeToPlayParams->PlaySpace,
        CameraShakeToPlayParams->UserPlaySpaceRot
    );

    return FReply::Handled();
}

FReply FCameraShakeEditor::StopCameraShake()
{
    if (IsValid(CameraShakeToPlay))
    {
        bool bImmediately = true;
        CameraShakeToPlay->StopShake(bImmediately);
    }

    return FReply::Handled();
}

FReply FCameraShakeEditor::ResetCamera()
{
    FCameraShakeEditorViewportClient& ViewportClient = Viewport->GetViewportClient();

    ViewportClient.ResetCamera();

    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
