#pragma once

#include "src/pch/pch.h"
#include "src/managers/ShaderManager.h"


HNCRSP_NAMESPACE_START

class PostprocessingManager
{
private:
    std::unordered_map< uint32_t, std::shared_ptr<Shader> > umap;
};

HNCRSP_NAMESPACE_END