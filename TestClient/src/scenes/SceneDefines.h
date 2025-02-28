#pragma once

#include "api/scene/Scene.h"

template <typename T>
using RenderObj = std::unique_ptr<Honeycrisp::SceneRenderObj<T>>;