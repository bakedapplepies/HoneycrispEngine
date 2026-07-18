#version 460 core

// G-Buffer textures
layout (location = 0) out vec3 HDR;
layout (location = 1) out vec3 Bloom;

// Constants
const float PI = 3.1415926;

const uint UBO_BINDING_GLOBAL   = 0;
const uint UBO_BINDING_LIGHT    = 1;
const uint UBO_BINDING_MATERIAL = 2;

const uint TEXTURE_UNIT_ALBEDO    = 0;
const uint TEXTURE_UNIT_NORMAL    = 1;
const uint TEXTURE_UNIT_ROUGHNESS = 2;
const uint TEXTURE_UNIT_CUBEMAP   = 3;
const uint TEXTURE_UNIT_DEPTH_MAP = 4;

const uint MAX_POINT_LIGHTS = 20;

// Data from Vertex stage
layout (location = 0) in V_OUT
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 UV;
    vec4 LightSpacePosition;
    flat uint instanceID;
} v_out;

// Textures
layout (binding = TEXTURE_UNIT_ALBEDO) uniform sampler2D u_albedo;
layout (binding = TEXTURE_UNIT_NORMAL) uniform sampler2D u_normal;
layout (binding = TEXTURE_UNIT_ROUGHNESS) uniform sampler2D u_roughness;
layout (binding = TEXTURE_UNIT_CUBEMAP) uniform samplerCube u_cubemap;
layout (binding = TEXTURE_UNIT_DEPTH_MAP) uniform sampler2D u_depthMap;

void main()
{
    
}