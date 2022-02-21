// Copyright 2020 Peter Leontev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Camera/CameraTypes.h"

#include "CameraShakeLibrary.generated.h"


class UCameraShakeBase;
class UCameraShakeBase;
class APlayerController;


UCLASS()
class CAMERASHAKERUNTIME_API UCameraShakeLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "CameraShake")
    static bool PlayCameraShake(APlayerController* PlayerController, UCameraShakeBase* Shake, float Scale = 1.f, ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal, FRotator UserPlaySpaceRot = FRotator::ZeroRotator);

    static void CopyCameraShakeParams(UCameraShakeBase* Source, UCameraShakeBase* Target);
};
