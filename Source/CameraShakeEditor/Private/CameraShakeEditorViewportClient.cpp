// Copyright 2020 Peter Leontev. All Rights Reserved.

#include "CameraShakeEditorViewportClient.h"
#include "EngineGlobals.h"
#include "Editor.h"
#include "Engine/Canvas.h"
#include "UnrealEngine.h"
#include "SEditorViewport.h"
#include "PreviewScene.h"
#include "Editor/EditorEngine.h"
#include "MatineeCameraShake.h"

#include "ICameraShakeEditor.h"
#include "SCameraShakeEditorViewport.h"

#define LOCTEXT_NAMESPACE "FCameraShakeEditorViewportClient"

FCameraShakeEditorViewportClient::FCameraShakeEditorViewportClient(
    TWeakPtr<ICameraShakeEditor> InCameraShakeEditor,
    const TSharedRef<SCameraShakeEditorViewport>& InCameraShakeEditorViewport,
    const TSharedRef<FPreviewScene>& InPreviewScene,
    UCameraShakeBase* InCameraShake,
    const FMinimalViewInfo& InViewInfo
) : FEditorViewportClient(
        nullptr,
        &InPreviewScene.Get(),
        StaticCastSharedRef<SEditorViewport>(InCameraShakeEditorViewport)
    )
    , InitialViewInfo(InViewInfo)
    , CameraShakeEditorPtr(InCameraShakeEditor)
    , CameraShakeEditorViewportPtr(InCameraShakeEditorViewport)
{
    PreviewScene = &InPreviewScene.Get();

	CameraShake = InCameraShake;
    {
        ResetCamera();
    }


    EngineShowFlags = FEngineShowFlags(ESFIM_Editor);
    EngineShowFlags.SetEditor(false);

    SetViewMode(VMI_Lit);
}


FCameraShakeEditorViewportClient::~FCameraShakeEditorViewportClient()
{
}

void FCameraShakeEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

}

bool FCameraShakeEditorViewportClient::InputWidgetDelta( FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale )
{
    bool bHandled = false;
    return bHandled;
}

UWorld* FCameraShakeEditorViewportClient::GetWorld() const
{
    UWorld* World = FEditorViewportClient::GetWorld();

    const TArray<FEditorViewportClient*>& AllViewportClients = GEditor->GetAllViewportClients();
    for (FEditorViewportClient* CurrViewportClient : AllViewportClients)
    {
        if ((FLevelEditorViewportClient*)CurrViewportClient != nullptr && IsValid(CurrViewportClient->GetWorld()))
        {
            World = CurrViewportClient->GetWorld();
            break;
        }
    }

    return World;
}

void FCameraShakeEditorViewportClient::ResetCamera()
{
    SetViewLocation(InitialViewInfo.Location);
    SetViewRotation(InitialViewInfo.Rotation);
    ViewFOV = InitialViewInfo.FOV;
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

	return bHandled;
}

bool FCameraShakeEditorViewportClient::InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	bool bResult = true;
	
	if (!bDisableInput)
	{
		bResult = FEditorViewportClient::InputAxis(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	return bResult;
}

void FCameraShakeEditorViewportClient::ProcessClick(class FSceneView& InView, class HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	Invalidate();
}

#undef LOCTEXT_NAMESPACE 