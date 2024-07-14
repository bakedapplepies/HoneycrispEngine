#pragma once

#include "src/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class Object  // TODO: Useless as of now. Maybe turn into some kind of SceneObj?
{
public:
    virtual ~Object() = default;
};

HNCRSP_NAMESPACE_END