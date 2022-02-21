// Copyright 2020 Peter Leontev. All Rights Reserved.

#include "SCameraShakeEditorViewport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "UObject/Package.h"
#include "EditorStyleSet.h"
#include "Slate/SceneViewport.h"
#include "LevelEditorViewport.h"
#include "ComponentReregisterContext.h"
#include "Widgets/Docking/SDockTab.h"
#include "SEditorViewportToolBarMenu.h"
#include "Editor.h"

#include "SCameraShakeEditorViewportToolBar.h"
#include "ICameraShakeEditor.h"
#include "CameraShakeEditorActions.h"

#define LOCTEXT_NAMESPACE "CameraShakeEditorViewport"

///////////////////////////////////////////////////////////
// SCameraShakeEditorViewport

void SCameraShakeEditorViewport::Construct(const FArguments& InArgs)
{
	// restore last used feature level
	UWorld* World = PreviewScene->GetWorld();
	if (World != nullptr)
	{
		World->ChangeFeatureLevel(GWorld->FeatureLevel);
	}

	CameraShakeEditorPtr = InArgs._CameraShakeEditor;

	CameraShake = InArgs._ObjectToEdit;

	SEditorViewport::Construct( SEditorViewport::FArguments() );

	UpdateCameraShake(CameraShake);

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &SCameraShakeEditorViewport::OnObjectPropertyChanged);
}

void SCameraShakeEditorViewport::PopulateViewportOverlays(TSharedRef<SOverlay> Overlay)
{
	SEditorViewport::PopulateViewportOverlays(Overlay);

	Overlay->AddSlot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		.Padding(FMargin(10.0f, 40.0f, 10.0f, 10.0f))
		[
			SAssignNew(OverlayTextVerticalBox, SVerticalBox)
		];

	// this widget will display the current viewed feature level
	Overlay->AddSlot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Right)
		.Padding(5.0f)
		[
			BuildFeatureLevelWidget()
		];
}

SCameraShakeEditorViewport::SCameraShakeEditorViewport()
	: PreviewScene(MakeShareable(new FPreviewScene(FPreviewScene::ConstructionValues())))
{

}

SCameraShakeEditorViewport::~SCameraShakeEditorViewport()
{
	CastChecked<UEditorEngine>(GEngine)->OnPreviewFeatureLevelChanged().Remove(PreviewFeatureLevelChangedHandle);

	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
	if (EditorViewportClient.IsValid())
	{
		EditorViewportClient->Viewport = NULL;
	}
}

void SCameraShakeEditorViewport::PopulateOverlayText(const TArray<FOverlayTextItem>& TextItems)
{
	OverlayTextVerticalBox->ClearChildren();

	for (const auto& TextItem : TextItems)
	{
		OverlayTextVerticalBox->AddSlot()
		[
			SNew(STextBlock)
			.Text(TextItem.Text)
			.TextStyle(FEditorStyle::Get(), TextItem.Style)
		];
	}
}

TSharedRef<class SEditorViewport> SCameraShakeEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SCameraShakeEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SCameraShakeEditorViewport::OnFloatingButtonClicked()
{
}

void SCameraShakeEditorViewport::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( CameraShake );
}

void SCameraShakeEditorViewport::RefreshViewport()
{
	// Invalidate the viewport's display.
	SceneViewport->Invalidate();
	EditorViewportClient->Invalidate();
}

void SCameraShakeEditorViewport::OnObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
{
	if ( !ensure(ObjectBeingModified) )
	{
		return;
	}

    RefreshViewport();
}

void SCameraShakeEditorViewport::UpdateCameraShake(UCameraShakeBase* InCameraShake, bool bResetCamera/*= true*/)
{
	EditorViewportClient->ResetCamera();
}

bool SCameraShakeEditorViewport::IsVisible() const
{
	return ViewportWidget.IsValid() && (!ParentTab.IsValid() || ParentTab.Pin()->IsForeground());
}

FCameraShakeEditorViewportClient& SCameraShakeEditorViewport::GetViewportClient()
{
	return *EditorViewportClient;
}


TSharedRef<FEditorViewportClient> SCameraShakeEditorViewport::MakeEditorViewportClient()
{
    FMinimalViewInfo ViewInfo;
	
	if (GEditor->GetLevelViewportClients().Num() > 0)
    {
        const FLevelEditorViewportClient* ViewportClient = GEditor->GetLevelViewportClients()[1];
        if (ViewportClient != nullptr)
        {
			ViewInfo.Location = ViewportClient->GetViewLocation();
			ViewInfo.Rotation = ViewportClient->GetViewRotation();
			ViewInfo.FOV = ViewportClient->ViewFOV;
        }
    }
	
	EditorViewportClient = MakeShareable(
        new FCameraShakeEditorViewportClient(
            CameraShakeEditorPtr,
            SharedThis(this),
            PreviewScene.ToSharedRef(),
            CameraShake,
			ViewInfo
        ) 
    );

	EditorViewportClient->bSetListenerPosition = false;

	EditorViewportClient->SetRealtime( true );
	EditorViewportClient->VisibilityDelegate.BindSP( this, &SCameraShakeEditorViewport::IsVisible );

	return EditorViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SCameraShakeEditorViewport::MakeViewportToolbar()
{
	return SNew(SCameraShakeEditorViewportToolbar, SharedThis(this));
}

EVisibility SCameraShakeEditorViewport::OnGetViewportContentVisibility() const
{
	return IsVisible() ? EVisibility::Visible : EVisibility::Collapsed;
}

void SCameraShakeEditorViewport::BindCommands()
{
	SEditorViewport::BindCommands();

}

#undef LOCTEXT_NAMESPACE