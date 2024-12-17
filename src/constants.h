#pragma once

#include "src/utils/utils.h"


HNCRSP_NAMESPACE_START

// Vertex Array-related constants ----------
extern const uint8_t VERTEX_ATTRIB_POSITION_INDEX;
extern const uint8_t VERTEX_ATTRIB_COLOR_INDEX;
extern const uint8_t VERTEX_ATTRIB_UV_INDEX;
extern const uint8_t VERTEX_ATTRIB_NORMAL_INDEX;

extern const uint16_t VERTEX_ATTRIB_POSITION_BIT;
extern const uint16_t VERTEX_ATTRIB_COLOR_BIT;
extern const uint16_t VERTEX_ATTRIB_UV_BIT;
extern const uint16_t VERTEX_ATTRIB_NORMAL_BIT;
// ----------

// Shader-related constants ----------
extern const uint8_t VERTEX_SHADER_BIT;
extern const uint8_t FRAGMENT_SHADER_BIT;
extern const uint8_t GEOMETRY_SHADER_BIT;
extern const uint8_t TESSELATION_SHADER_BIT;
extern const uint8_t COMPUTE_SHADER_BIT;
// ----------

// Texture types ----------
// Also acts as Texture Units (so it has to be non 'enum class')
enum ETextureType : int8_t
{
    NONE = -1,
    ALBEDO,
    ROUGHNESS,
    AO,
    NORMAL,
    SPECULAR
};
extern const uint16_t DEPTH_BUFFER_TEXTURE_UNIT_INDEX;
extern const uint16_t DEPTH_BUFFER_NO_ALPHA_TEXTURE_UNIT_INDEX;
extern const uint16_t COLOR_BUFFER_TEXTURE_UNIT_INDEX;
extern const uint16_t CUBEMAP_TEXTURE_UNIT_INDEX;
// ----------

// Scene lights ----------
constexpr uint32_t MAX_DIRECTIONAL_LIGHTS = 1;
constexpr uint32_t MAX_POINT_LIGHTS = 10;
constexpr uint32_t MAX_SPOT_LIGHTS = 1;
// ----------

// Vectors ----------
extern const glm::vec3 VEC3_UP;
extern const glm::vec3 VEC3_DOWN;

HNCRSP_NAMESPACE_END