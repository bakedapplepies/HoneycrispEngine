#pragma once

#include <Tracy.hpp>


#ifdef HNCRSP_TRACY_PROFILE
    #define HNCRSP_PROFILE ZoneScoped
    #define HNCRSP_PROFILE_FRAME FrameMark
#else
    #define HNCRSP_PROFILE
    #define HNCRSP_PROFILE_FRAME
#endif