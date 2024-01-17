#pragma once

#include "src/pch/pch.h"


#define TERMINATE(msg) terminate(msg, __FILE__, __LINE__);

void terminate(const char* msg, const char* file, unsigned int line);