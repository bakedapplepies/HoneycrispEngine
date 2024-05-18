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

// Scene lights ----------
const uint32_t MAX_DIRECTIONAL_LIGHTS = 1;
const uint32_t MAX_POINT_LIGHTS = 10;
const uint32_t MAX_SPOT_LIGHTS = 1;
// ----------

HNCRSP_NAMESPACE_END