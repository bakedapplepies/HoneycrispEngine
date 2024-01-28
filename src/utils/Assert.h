#pragma once

#include "Debug.h"


#ifdef HNCRSP_DEBUG
    #define ASSERT(x) if (!(x)) createBreak();

    void createBreak();
#else
    #define ASSERT(x) x;
#endif