#pragma once

#include "api/pch/hncrsp_pch.h"


HNCRSP_NAMESPACE_START

class Texture2DArray
{
private:
    GLuint m_textureID;

public:
    Texture2DArray(const std::vector<FileSystem::Path>& texturePaths);
    
};

HNCRSP_NAMESPACE_END