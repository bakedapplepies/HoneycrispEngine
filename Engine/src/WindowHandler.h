#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/core/Utils.h"

HNCRSP_NAMESPACE_START

class WindowHandler
{
private:
    CallbackData m_callbackData;

public:
    void StartUp();
    void ShutDown();
};

HNCRSP_NAMESPACE_END