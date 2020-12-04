// Copyright 2020 Peter Leontev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraTypes.h"
#include "CameraShakePlayParams.generated.h"


class UCameraShakeBase;


UCLASS()
class UCameraShakePlayParams : public UObject
{
	GENERATED_BODY()

public:
    UPROPERTY(Transient, EditAnywhere, Category = CameraShakePlayParams)
	ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = CameraShakePlayParams)
	float ShakeScale = 1.0f;

    UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = CameraShakePlayParams)
    FRotator UserPlaySpaceRot = FRotator::ZeroRotator;
};



