// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShake.h"
#include "Camera/CameraModifier_CameraShake.h"


bool UCameraShakeLibrary::PlayCameraShake(APlayerController* PlayerController, UCameraShake* Shake, float Scale, ECameraAnimPlaySpace::Type PlaySpace, FRotator UserPlaySpaceRot)
{
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

    CameraShakeModifier->ActiveShakes.Add(Shake);
    Shake->PlayShake(PlayerCameraManager, Scale, PlaySpace, UserPlaySpaceRot);

    return true;
}
