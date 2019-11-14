#include "CameraShakeFactory.h"
#include "Camera/CameraShake.h"
#include "Internationalization/Internationalization.h"


UCameraShakeFactory::UCameraShakeFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UCameraShake::StaticClass();
	bCreateNew = true;
}

FText UCameraShakeFactory::GetDisplayName() const
{
	return FText::FromString("Camera Shake");
}

FName UCameraShakeFactory::GetNewAssetThumbnailOverride() const
{
	return TEXT("ClassThumbnail.CameraShake");
}

uint32 UCameraShakeFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

UObject* UCameraShakeFactory::FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn)
{
	UCameraShake* NewCameraShake = NewObject<UCameraShake>(InParent, Class, Name, Flags);
	return NewCameraShake;
}