#include "src/constants.h"


HNCRSP_NAMESPACE_START

// Vertex Array-related constants ----------
const uint8_t VERTEX_ATTRIB_POSITION_INDEX = 0;
const uint8_t VERTEX_ATTRIB_COLOR_INDEX    = 1;
const uint8_t VERTEX_ATTRIB_UV_INDEX       = 2;
const uint8_t VERTEX_ATTRIB_NORMAL_INDEX   = 3;

const uint16_t VERTEX_ATTRIB_POSITION_BIT = 1 << VERTEX_ATTRIB_POSITION_INDEX;
const uint16_t VERTEX_ATTRIB_COLOR_BIT    = 1 << VERTEX_ATTRIB_COLOR_INDEX;
const uint16_t VERTEX_ATTRIB_UV_BIT       = 1 << VERTEX_ATTRIB_UV_INDEX;
const uint16_t VERTEX_ATTRIB_NORMAL_BIT   = 1 << VERTEX_ATTRIB_NORMAL_INDEX;
// ----------

// Shader-related constants
extern const uint8_t VERTEX_SHADER      = 0 << 1;
extern const uint8_t FRAGMENT_SHADER    = 0 << 2;
extern const uint8_t GEOMETRY_SHADER    = 0 << 3;
extern const uint8_t TESSELATION_SHADER = 0 << 4;
extern const uint8_t COMPUTE_SHADER     = 0 << 5;
// ----------

// Texture types ----------
extern const uint16_t COLOR_BUFFER_TEXTURE_UNIT_INDEX = 14;
extern const uint16_t CUBEMAP_TEXTURE_UNIT_INDEX = 15;
// ----------

// Scene lights ----------
const uint32_t MAX_DIRECTIONAL_LIGHTS = 1;
const uint32_t MAX_POINT_LIGHTS       = 10;
const uint32_t MAX_SPOT_LIGHTS        = 1;
// ----------

HNCRSP_NAMESPACE_END