// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CameraShakeEditorViewportClient.h"
#include "EngineGlobals.h"
#include "Settings/LevelEditorViewportSettings.h"
#include "Editor.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Components/LineBatchComponent.h"
#include "Engine/Canvas.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"
#include "Utils.h"
#include "UnrealEngine.h"
#include "SEditorViewport.h"
#include "PreviewScene.h"
#include "Engine/AssetUserData.h"
#include "Editor/EditorEngine.h"

#include "ICameraShakeEditor.h"
#include "SCameraShakeEditorViewport.h"

#define LOCTEXT_NAMESPACE "FCameraShakeEditorViewportClient"

FCameraShakeEditorViewportClient::FCameraShakeEditorViewportClient(
    TWeakPtr<ICameraShakeEditor> InCameraShakeEditor,
    const TSharedRef<SCameraShakeEditorViewport>& InCameraShakeEditorViewport,
    const TSharedRef<FPreviewScene>& InPreviewScene,
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
    PreviewScene = &InPreviewScene.Get();

	SetCameraShake(InCameraShake);

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
    SetViewLocation(FVector::ZeroVector);
    SetViewRotation(FRotator::ZeroRotator);

    Invalidate();
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


void FCameraShakeEditorViewportClient::SetCameraShake(UCameraShake* InCameraShake, bool bResetCamera/* =true */)
{
    CameraShake = InCameraShake;

	
	if (bResetCamera)
    {

	}
}

#undef LOCTEXT_NAMESPACE 