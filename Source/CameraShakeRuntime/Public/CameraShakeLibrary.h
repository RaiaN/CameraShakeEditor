// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Camera/CameraTypes.h"

#include "CameraShakeLibrary.generated.h"


class UCameraShake;
class APlayerController;


UCLASS()
class CAMERASHAKERUNTIME_API UCameraShakeLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "CameraShake")
    static bool PlayCameraShake(APlayerController* PlayerController, UCameraShake* Shake, float Scale = 1.f, ECameraAnimPlaySpace::Type PlaySpace = ECameraAnimPlaySpace::CameraLocal, FRotator UserPlaySpaceRot = FRotator::ZeroRotator);
};
