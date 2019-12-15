// Copyright 2019 Peter Leontev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"
#include "Engine/Blueprint.h"


class FAssetTypeActions_CameraShake : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_CameraShake", "Camera Shake"); }
	virtual FColor GetTypeColor() const override { return FColor(0, 255, 255); }
	virtual UClass* GetSupportedClass() const override { return UCameraShake::StaticClass(); }
	virtual bool HasActions( const TArray<UObject*>& InObjects ) const override { return true; }
	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Basic; }
	virtual bool IsImportedAsset() const override { return true; }
	// End IAssetTypeActions
};
