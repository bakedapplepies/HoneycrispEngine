#pragma once

#include "pch/pch.h"

#include "Texture.h"
#include "Mesh.h"


class Cube : public Mesh
{
public:
    Cube();
    Cube(const Cube& other) = delete;
    Cube(Cube&& other) noexcept;
    Cube& operator=(const Cube& other) = delete;
    Cube& operator=(Cube&& other) noexcept;
    ~Cube() = default;

private:
    void InitializeAttributeData();
};