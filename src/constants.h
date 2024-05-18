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

// Scene lights ----------
extern const uint32_t MAX_DIRECTIONAL_LIGHTS;
extern const uint32_t MAX_POINT_LIGHTS;
extern const uint32_t MAX_SPOT_LIGHTS;
// ----------

HNCRSP_NAMESPACE_END