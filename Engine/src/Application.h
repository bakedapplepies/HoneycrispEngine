#pragma once

#include "api/core/Defines.h"

HNCRSP_NAMESPACE_START

// TODO: maybe move these somewhere else
extern void Application_RegisterComponents();
extern void Application_RegisterSystems();
// extern void Application_RegisterScenes();

namespace Application
{
	void Run();	
}

HNCRSP_NAMESPACE_END