// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CameraShakeEditorViewportClient.h"
#include "EngineGlobals.h"
#include "Settings/LevelEditorViewportSettings.h"
#include "Editor.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Engine/Canvas.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"
#include "Utils.h"
#include "UnrealEngine.h"
#include "SEditorViewport.h"
#include "AdvancedPreviewScene.h"
#include "Runtime/Analytics/Analytics/Public/Interfaces/IAnalyticsProvider.h"
#include "EngineAnalytics.h"
#include "Engine/AssetUserData.h"
#include "Editor/EditorPerProjectUserSettings.h"

#include "ICameraShakeEditor.h"
#include "SCameraShakeEditorViewport.h"

#define LOCTEXT_NAMESPACE "FCameraShakeEditorViewportClient"

FCameraShakeEditorViewportClient::FCameraShakeEditorViewportClient(
    TWeakPtr<ICameraShakeEditor> InCameraShakeEditor,
    const TSharedRef<SCameraShakeEditorViewport>& InCameraShakeEditorViewport,
    const TSharedRef<FAdvancedPreviewScene>& InPreviewScene,
    UCameraShake* InCameraShake
) : 
    FEditorViewportClient(
        nullptr,
        &InPreviewScene.Get(),
        StaticCastSharedRef<SEditorViewport>(InCameraShakeEditorViewport)
    )
	, CameraShakeEditorPtr(InCameraShakeEditor)
	, CameraShakeEditorViewportPtr(InCameraShakeEditorViewport)
{
	// Setup defaults for the common draw helper.
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = true;
	DrawHelper.GridColorAxis = FColor(160,160,160);
	DrawHelper.GridColorMajor = FColor(144,144,144);
	DrawHelper.GridColorMinor = FColor(128,128,128);

	SetViewMode(VMI_Lit);

	OverrideNearClipPlane(1.0f);

	AdvancedPreviewScene = static_cast<FAdvancedPreviewScene*>(PreviewScene);

	SetCameraShake(InCameraShake);
}

FCameraShakeEditorViewportClient::~FCameraShakeEditorViewportClient()
{
}

void FCameraShakeEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);



	// Tick the preview scene world.
	if (!GIntraFrameDebuggingGameThread)
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

bool FCameraShakeEditorViewportClient::InputWidgetDelta( FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale )
{
    bool bHandled = false;
    return bHandled;
}

void FCameraShakeEditorViewportClient::TrackingStarted( const struct FInputEventState& InInputState, bool bIsDraggingWidget, bool bNudge )
{
	
}


void FCameraShakeEditorViewportClient::TrackingStopped()
{
	
}


void FCameraShakeEditorViewportClient::ResetCamera()
{
    SetViewLocation(FVector::ZeroVector);
    SetViewRotation(FRotator::ZeroRotator);

    Invalidate();
}

void FCameraShakeEditorViewportClient::Draw(const FSceneView* View,FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);

}

void FCameraShakeEditorViewportClient::DrawCanvas( FViewport& InViewport, FSceneView& View, FCanvas& Canvas )
{

	FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);
}


void FCameraShakeEditorViewportClient::MouseMove(FViewport* InViewport,int32 x, int32 y)
{
	FEditorViewportClient::MouseMove(InViewport,x,y);
}

bool FCameraShakeEditorViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event,float AmountDepressed, bool Gamepad)
{
	bool bHandled = FEditorViewportClient::InputKey(InViewport, ControllerId, Key, Event, AmountDepressed, false);

	// Handle viewport screenshot.
	bHandled |= InputTakeScreenshot( InViewport, Key, Event );

	bHandled |= AdvancedPreviewScene->HandleInputKey(InViewport, ControllerId, Key, Event, AmountDepressed, Gamepad);

	return bHandled;
}

bool FCameraShakeEditorViewportClient::InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	bool bResult = true;
	
	if (!bDisableInput)
	{
		bResult = AdvancedPreviewScene->HandleViewportInput(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
		if (bResult)
		{
			Invalidate();
		}
		else
		{
			bResult = FEditorViewportClient::InputAxis(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}

	return bResult;
}

void FCameraShakeEditorViewportClient::ProcessClick(class FSceneView& InView, class HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{

	Invalidate();
}


void FCameraShakeEditorViewportClient::SetFloorAndEnvironmentVisibility(const bool bVisible)
{
	AdvancedPreviewScene->SetFloorVisibility(bVisible, true);
	AdvancedPreviewScene->SetEnvironmentVisibility(bVisible, true);
}

void FCameraShakeEditorViewportClient::SetCameraShake(UCameraShake* InCameraShake, bool bResetCamera/* =true */)
{
    CameraShake = InCameraShake;

	
	if (bResetCamera)
    {

	}
}

#undef LOCTEXT_NAMESPACE 