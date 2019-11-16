#pragma once

#include "IDetailCustomization.h"
#include "CameraShakeEditor.h"

class FCameraShakeDetails : public IDetailCustomization
{
public:
	FCameraShakeDetails( class FCameraShakeEditor& InCameraShakeEditor );
	~FCameraShakeDetails();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails( class IDetailLayoutBuilder& DetailBuilder ) override;
private:
	/** Static mesh editor */
	class FCameraShakeEditor& CameraShakeEditor;

};