#pragma once

#include "pch/pch.h"
#include "Mesh.h"
#include "Debug.h"


class Model : public Mesh
{
public:
    Model(const std::string& path);
};