#include "api/core/Utils.h"

HNCRSP_NAMESPACE_START

CallbackData* GetCallbackData()
{
    return static_cast<CallbackData*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
}

HNCRSP_NAMESPACE_END