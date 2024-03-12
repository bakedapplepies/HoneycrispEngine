#pragma once

#include "src/ecs/ECSManager.h"


HNCRSP_NAMESPACE_START

void Application_RegisterComponents();
void Application_RegisterSystems();

namespace Application
{
	void Run();	
}

HNCRSP_NAMESPACE_END