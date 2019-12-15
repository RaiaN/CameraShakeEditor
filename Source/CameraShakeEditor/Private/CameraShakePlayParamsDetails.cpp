// Copyright 2019 Peter Leontev. All Rights Reserved.

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