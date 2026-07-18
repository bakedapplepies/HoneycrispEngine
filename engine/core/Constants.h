#pragma once

#include "Defines.h"

HNCRSP_NAMESPACE_START

constexpr int UBO_BINDING_INDEX_GLOBAL      = 0;
constexpr int UBO_BINDING_INDEX_LIGHT       = 1;
constexpr int UBO_BINDING_INDEX_MATERIAL    = 2;
constexpr int UBO_BINDING_INDEX_POSTPROCESS = 3;

constexpr int TEXTURE_UNIT_ALBEDO    = 0;
constexpr int TEXTURE_UNIT_NORMAL    = 1;
constexpr int TEXTURE_UNIT_ROUGHNESS = 2;
constexpr int TEXTURE_UNIT_CUBEMAP   = 3;
constexpr int TEXTURE_UNIT_DEPTH_MAP = 4;

constexpr int TEXTURE_UNIT_GBUFFER_POSITION  = 0;
constexpr int TEXTURE_UNIT_GBUFFER_NORMAL    = 1;
constexpr int TEXTURE_UNIT_GBUFFER_ROUGHNESS = 2;
constexpr int TEXTURE_UNIT_GBUFFER_ALBEDO    = 3;

constexpr int IMAGE_UNIT_READ       = 0;
constexpr int IMAGE_UNIT_WRITE      = 1;
constexpr int IMAGE_UNIT_READ_WRITE = 2;

HNCRSP_NAMESPACE_END