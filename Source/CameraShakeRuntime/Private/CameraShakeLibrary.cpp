// Copyright 2020 Peter Leontev. All Rights Reserved.

#include "CameraShakeLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShake.h"
#include "Camera/CameraModifier_CameraShake.h"


bool UCameraShakeLibrary::PlayCameraShake(APlayerController* PlayerController, UCameraShakeBase* Shake, float Scale, ECameraShakePlaySpace PlaySpace, FRotator UserPlaySpaceRot)
{
    if (!IsValid(Shake))
    {
        return false;
    }
    
    APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
    if (!IsValid(PlayerCameraManager))
    {
        return false;
    }    

    UCameraModifier_CameraShake* CameraShakeModifier = Cast<UCameraModifier_CameraShake>(
        PlayerCameraManager->FindCameraModifierByClass(UCameraModifier_CameraShake::StaticClass())
    );

    if (!IsValid(CameraShakeModifier))
    {
        CameraShakeModifier = Cast<UCameraModifier_CameraShake>(
            PlayerCameraManager->AddNewCameraModifier(UCameraModifier_CameraShake::StaticClass())
        );
    }

    UCameraShakeBase* ShakeToPlay = NewObject<UCameraShakeBase>(CameraShakeModifier);
    CopyCameraShakeParams(Shake, ShakeToPlay);

    CameraShakeModifier->AddCameraShake(Shake->GetClass(), FAddCameraShakeParams());
    ShakeToPlay->StartShake(PlayerCameraManager, Scale, PlaySpace, UserPlaySpaceRot);

    return true;
}

void UCameraShakeLibrary::CopyCameraShakeParams(UCameraShakeBase* InSource, UCameraShakeBase* InTarget)
{
    UMatineeCameraShake* Source = Cast<UMatineeCameraShake>(InSource);
    UMatineeCameraShake* Target = Cast<UMatineeCameraShake>(InTarget);
    
    checkSlow(Source);
    checkSlow(Target);

    Target->bSingleInstance = Source->bSingleInstance;
    Target->OscillationDuration = Source->OscillationDuration;
    Target->OscillationBlendInTime = Source->OscillationBlendInTime;
    Target->OscillationBlendOutTime = Source->OscillationBlendOutTime;
    Target->RotOscillation = Source->RotOscillation;
    Target->LocOscillation = Source->LocOscillation;
    Target->FOVOscillation = Source->FOVOscillation;
}
