#include "CameraShakeDetails.h"

#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "CameraShakeEditor"

FCameraShakeDetails::FCameraShakeDetails(class FCameraShakeEditor& InCameraShakeEditor)
    : CameraShakeEditor(InCameraShakeEditor)
{}

FCameraShakeDetails::~FCameraShakeDetails()
{
}

void FCameraShakeDetails::CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& CameraShakeCategory = DetailBuilder.EditCategory("StaticMesh", LOCTEXT("CameraShakeGeneralSettings", "General Settings"));

 
}

#undef LOCTEXT_NAMESPACE