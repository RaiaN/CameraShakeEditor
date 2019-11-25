#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraTypes.h"
#include "CameraShakePlayParams.generated.h"


class UCameraShake;


UCLASS()
class UCameraShakePlayParams : public UObject
{
	GENERATED_BODY()

public:
    UPROPERTY(Transient, EditAnywhere, Category = CameraShakePlayParams)
	TEnumAsByte<ECameraAnimPlaySpace::Type> PlaySpace = ECameraAnimPlaySpace::CameraLocal;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = CameraShakePlayParams)
	float ShakeScale = 1.0f;

    UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = CameraShakePlayParams)
    FRotator UserPlaySpaceRot = FRotator::ZeroRotator;
};



