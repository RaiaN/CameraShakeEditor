// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "UnrealWidget.h"
#include "EditorViewportClient.h"
#include "Components.h"
#include "Interfaces/Interface_CollisionDataProvider.h"

class FAdvancedPreviewScene;
class FCanvas;
class ICameraShakeEditor;
class SCameraShakeEditorViewport;
class UCameraShake;


/** Viewport Client for the preview viewport */
class FCameraShakeEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FCameraShakeEditorViewportClient>
{
public:
    FCameraShakeEditorViewportClient(TWeakPtr<ICameraShakeEditor> InCameraShakeEditor, const TSharedRef<SCameraShakeEditorViewport>& InCameraShakeEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene, UCameraShake* InCameraShake);
	~FCameraShakeEditorViewportClient();

	// FEditorViewportClient interface
	virtual void MouseMove(FViewport* Viewport,int32 x, int32 y) override;
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.f, bool bGamepad=false) override;
	virtual bool InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples=1, bool bGamepad=false) override;
	virtual void ProcessClick(class FSceneView& View, class HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;
	virtual void Tick(float DeltaSeconds) override;
    virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas) override;
	virtual bool InputWidgetDelta( FViewport* Viewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale ) override;
	virtual void TrackingStarted( const struct FInputEventState& InInputState, bool bIsDragging, bool bNudge ) override;
	virtual void TrackingStopped() override;

	void SetCameraShake(UCameraShake* InCameraShake, bool bResetCamera=true);

	/** Used to toggle the floor when vertex colours should be shown */
	void SetFloorAndEnvironmentVisibility(const bool bVisible);

private:
	/** The camera shake being used in the editor. */
	UCameraShake* CameraShake;

	/** Pointer back to the CameraShake editor tool that owns us */
	TWeakPtr<ICameraShakeEditor> CameraShakeEditorPtr;

	/** Pointer back to the CameraShakeEditor viewport control that owns us */
	TWeakPtr<SCameraShakeEditorViewport> CameraShakeEditorViewportPtr;

	/** Stored pointer to the preview scene in which the camera shake is shown */
	FAdvancedPreviewScene* AdvancedPreviewScene;
};
