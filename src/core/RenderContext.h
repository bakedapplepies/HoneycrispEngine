#pragma once

#include "src/pch/pch.h"

#include "src/types/Singleton.h"


HNCRSP_NAMESPACE_START

namespace RenderContext
{
    namespace
    {
        bool m_alreadyStarted = false;
    }
    void StartUp();
    void ShutDown();
}

HNCRSP_NAMESPACE_END