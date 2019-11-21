// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "EditorViewportClient.h"
#include "CameraShakeEditorViewportClient.h"
#include "PreviewScene.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"

class ICameraShakeEditor;
class SVerticalBox;

/**
 * CameraShake Editor Preview viewport widget
 */
class SCameraShakeEditorViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SCameraShakeEditorViewport){}
		SLATE_ARGUMENT(TWeakPtr<ICameraShakeEditor>, CameraShakeEditor)
		SLATE_ARGUMENT(UCameraShake*, ObjectToEdit)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
    SCameraShakeEditorViewport();
	~SCameraShakeEditorViewport();
	
	// FGCObject interface
	virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;
	// End of FGCObject interface

	/** Constructs, destroys, and updates preview mesh components based on the preview camera shake's sockets. */
	void RefreshViewport();


	void UpdateCameraShake(UCameraShake* InCameraShake, bool bResetCamera=true);

	/** @return The editor viewport client */
	class FCameraShakeEditorViewportClient& GetViewportClient();

	/** Set the parent tab of the viewport for determining visibility */
	void SetParentTab( TSharedRef<SDockTab> InParentTab ) { ParentTab = InParentTab; }

	/** Struct defining the text and its style of each item in the overlay widget */
	struct FOverlayTextItem
	{
		explicit FOverlayTextItem(const FText& InText, const FName& InStyle = "TextBlock.ShadowedText")
			: Text(InText), Style(InStyle)
		{}

		FText Text;
		FName Style;
	};

	/** Specifies an array of text items which will be added to the viewport overlay */
	void PopulateOverlayText( const TArray<FOverlayTextItem>& TextItems );

	// ICommonEditorViewportToolbarInfoProvider interface
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;
	// End of ICommonEditorViewportToolbarInfoProvider interface

	/** Returns the preview scene being renderd in the viewport */
	TSharedRef<FPreviewScene> GetPreviewScene() { return PreviewScene.ToSharedRef(); }
protected:
	/** SEditorViewport interface */
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual EVisibility OnGetViewportContentVisibility() const override;
	virtual void BindCommands() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;
	virtual void PopulateViewportOverlays(TSharedRef<SOverlay> Overlay) override;

private:
	/** Determines the visibility of the viewport. */
	bool IsVisible() const override;

	/** Callback for updating preview socket meshes if the camera shake has been modified. */
	void OnObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent);
private:
	
	/** The parent tab where this viewport resides */
	TWeakPtr<SDockTab> ParentTab;

	/** Pointer back to the CameraShake editor tool that owns us */
	TWeakPtr<ICameraShakeEditor> CameraShakeEditorPtr;

	/** The scene for this viewport. */
	TSharedPtr<FPreviewScene> PreviewScene;

	/** Editor viewport client */
	TSharedPtr<class FCameraShakeEditorViewportClient> EditorViewportClient;

	/** Camera shake being edited */
	UCameraShake* CameraShake;

	/** Pointer to the vertical box into which the overlay text items are added */
	TSharedPtr<SVerticalBox> OverlayTextVerticalBox;

	/** Handle to the registered OnPreviewFeatureLevelChanged delegate. */
	FDelegateHandle PreviewFeatureLevelChangedHandle;
};
