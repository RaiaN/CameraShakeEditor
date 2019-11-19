#include "CameraShakeDetails.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/SlateDelegates.h"

#define LOCTEXT_NAMESPACE "CameraShakeEditor"

FCameraShakeDetails::FCameraShakeDetails(class FCameraShakeEditor& InCameraShakeEditor)
    : CameraShakeEditor(InCameraShakeEditor)
{}

FCameraShakeDetails::~FCameraShakeDetails()
{
}

void FCameraShakeDetails::CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& CameraShakeCategory = DetailBuilder.EditCategory("GeneralSettings", LOCTEXT("CameraShakeGeneralSettings", "General Settings"));

    DetailBuilder.HideCategory("AnimShake");

    
    const FText PlayCameraShakeText = LOCTEXT("PlayCameraShake", "Play");
    const FText StopCameraShakeText = LOCTEXT("StopCameraShake", "Stop");

    CameraShakeCategory.AddCustomRow(PlayCameraShakeText, false)
    .NameContent()
    [
        SNullWidget::NullWidget
    ]
    .ValueContent()
    .VAlign(VAlign_Center)
    .MaxDesiredWidth(250)
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(SButton)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .ToolTipText(LOCTEXT("PlayCameraShakeTooltip", "Plays camera shake"))
            .OnClicked(FOnClicked::CreateSP(&CameraShakeEditor, &FCameraShakeEditor::PlayCameraShake))
            .Content()
            [
                SNew(STextBlock)
                .Text(PlayCameraShakeText)
            ]
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(SButton)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            .ToolTipText(LOCTEXT("StopCameraShakeTooltip", "Stops currently active camera shake"))
            .OnClicked(FOnClicked::CreateSP(&CameraShakeEditor, &FCameraShakeEditor::StopCameraShake))
            .Content()
            [
                SNew(STextBlock)
                .Text(StopCameraShakeText)
            ]
        ]
    ];
}

#undef LOCTEXT_NAMESPACE