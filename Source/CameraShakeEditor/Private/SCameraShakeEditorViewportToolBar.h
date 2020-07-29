// Copyright 2020 Peter Leontev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SCameraShakeEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"


/**
 * A level viewport toolbar widget that is placed in a viewport
 */
class SCameraShakeEditorViewportToolbar : public SCommonEditorViewportToolbarBase
{
public:
	SLATE_BEGIN_ARGS(SCameraShakeEditorViewportToolbar) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the toolbar
	 */
	void Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider);
};

