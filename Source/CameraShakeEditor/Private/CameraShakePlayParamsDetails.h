// Copyright Peter Leontev, peterleontev.com

#pragma once

#include "IDetailCustomization.h"
#include "CameraShakeEditor.h"

class FCameraShakePlayParamsDetails : public IDetailCustomization
{
public:
    FCameraShakePlayParamsDetails( class FCameraShakeEditor& InCameraShakeEditor );
	~FCameraShakePlayParamsDetails();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails( class IDetailLayoutBuilder& DetailBuilder ) override;
private:
	/** Static mesh editor */
	class FCameraShakeEditor& CameraShakeEditor;

};