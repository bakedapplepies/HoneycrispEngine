#include "api/core/constants.h"


HNCRSP_NAMESPACE_START

// Vertex Array-related constants ----------
constexpr uint8_t VERTEX_ATTRIB_POSITION_INDEX = 0;
constexpr uint8_t VERTEX_ATTRIB_COLOR_INDEX    = 1;
constexpr uint8_t VERTEX_ATTRIB_UV_INDEX       = 2;
constexpr uint8_t VERTEX_ATTRIB_NORMAL_INDEX   = 3;

constexpr uint16_t VERTEX_ATTRIB_POSITION_BIT = 1 << VERTEX_ATTRIB_POSITION_INDEX;
constexpr uint16_t VERTEX_ATTRIB_COLOR_BIT    = 1 << VERTEX_ATTRIB_COLOR_INDEX;
constexpr uint16_t VERTEX_ATTRIB_UV_BIT       = 1 << VERTEX_ATTRIB_UV_INDEX;
constexpr uint16_t VERTEX_ATTRIB_NORMAL_BIT   = 1 << VERTEX_ATTRIB_NORMAL_INDEX;
// ----------

// Shader-related constants
extern constexpr uint8_t VERTEX_SHADER_BIT      = 0 << 1;
extern constexpr uint8_t FRAGMENT_SHADER_BIT    = 0 << 2;
extern constexpr uint8_t GEOMETRY_SHADER_BIT    = 0 << 3;
extern constexpr uint8_t TESSELATION_SHADER_BIT = 0 << 4;
extern constexpr uint8_t COMPUTE_SHADER_BIT     = 0 << 5;
// ----------

// Texture types ----------
extern constexpr uint16_t DEPTH_BUFFER_TEXTURE_UNIT_INDEX          = 12;
extern constexpr uint16_t DEPTH_BUFFER_NO_ALPHA_TEXTURE_UNIT_INDEX = 13;
extern constexpr uint16_t COLOR_BUFFER_TEXTURE_UNIT_INDEX          = 14;
extern constexpr uint16_t CUBEMAP_TEXTURE_UNIT_INDEX               = 15;
// ----------

// Vectors
extern constexpr glm::vec3 VEC3_UP = glm::vec3(0.0f, 1.0f, 0.0f);
extern constexpr glm::vec3 VEC3_DOWN = glm::vec3(0.0f, -1.0f, 0.0f);

HNCRSP_NAMESPACE_END