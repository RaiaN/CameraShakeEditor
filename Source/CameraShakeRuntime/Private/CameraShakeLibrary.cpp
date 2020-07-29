// Copyright 2019 Peter Leontev. All Rights Reserved.

#include "CameraShakeLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShake.h"
#include "Camera/CameraModifier_CameraShake.h"


bool UCameraShakeLibrary::PlayCameraShake(APlayerController* PlayerController, UCameraShake* Shake, float Scale, ECameraAnimPlaySpace::Type PlaySpace, FRotator UserPlaySpaceRot)
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

    UCameraShake* ShakeToPlay = NewObject<UCameraShake>(CameraShakeModifier);
    CopyCameraShakeParams(Shake, ShakeToPlay);

    CameraShakeModifier->AddCameraShake(Shake->GetClass(), FAddCameraShakeParams());
    ShakeToPlay->PlayShake(PlayerCameraManager, Scale, PlaySpace, UserPlaySpaceRot);

    return true;
}

void UCameraShakeLibrary::CopyCameraShakeParams(UCameraShake* Source, UCameraShake* Target)
{
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
