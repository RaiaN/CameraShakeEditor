// Copyright 2019 Peter Leontev. All Rights Reserved.

#pragma once

#include "ICameraShakeRuntimeModule.h"

class FCameraShakeRuntimeModule : public ICameraShakeRuntimeModule
{
public:
    FCameraShakeRuntimeModule();
    ~FCameraShakeRuntimeModule();

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};