// Copyright Peter Leontev, peterleontev.com

#include "CameraShakePlayParamsDetails.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"

#define LOCTEXT_NAMESPACE "CameraShakeEditor"

FCameraShakePlayParamsDetails::FCameraShakePlayParamsDetails(class FCameraShakeEditor& InCameraShakeEditor)
    : CameraShakeEditor(InCameraShakeEditor)
{}

FCameraShakePlayParamsDetails::~FCameraShakePlayParamsDetails()
{
}

void FCameraShakePlayParamsDetails::CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& CameraShakeCategory = DetailBuilder.EditCategory("PlayParams", LOCTEXT("CameraShakePlayParams", "Play Params"));

    
}

#undef LOCTEXT_NAMESPACE