#version 460 core

// G-Buffer textures
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

// Constants
const float PI = 3.1415926;

const uint UBO_BINDING_GLOBAL   = 0;
const uint UBO_BINDING_LIGHT    = 1;
const uint UBO_BINDING_MATERIAL = 2;

const uint TEXTURE_UNIT_0         = 0;
const uint TEXTURE_UNIT_1         = 1;
const uint TEXTURE_UNIT_2         = 2;
const uint TEXTURE_UNIT_CUBEMAP   = 3;
const uint TEXTURE_UNIT_DEPTH_MAP = 4;

const uint MAX_POINT_LIGHTS = 20;

// Structs
struct DirLight
{
    vec3 direction;
};

struct PointLight
{
    vec3 position;
    // float radius_thres;
};

// Data from Vertex stage
layout (location = 0) in V_OUT
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 UV;
    vec4 LightSpacePosition;
    flat uint instanceID;
} v_out;

// UBOs
layout (binding = UBO_BINDING_GLOBAL, std140) uniform GlobalUBO
{
    float u_time;
    mat4 u_view;
    mat4 u_projection;
    vec4 u_cameraPos;
    mat4 u_lightSpace;
};

layout (binding = UBO_BINDING_LIGHT, std140) uniform LightUBO
{
    DirLight u_dirLight;

    uint u_numPointLight;
    PointLight u_pointLights[MAX_POINT_LIGHTS];
};

layout (binding = UBO_BINDING_MATERIAL, std140) uniform MaterialUBO
{
    float u_ambient_intensity;
    float u_diffuse_intensity;
    float u_specular_intensity;
    float u_roughness_scalar;
};

// Textures
layout (binding = TEXTURE_UNIT_0) uniform sampler2D u_albedo;
layout (binding = TEXTURE_UNIT_1) uniform sampler2D u_normal;
layout (binding = TEXTURE_UNIT_2) uniform sampler2D u_roughness;
layout (binding = TEXTURE_UNIT_CUBEMAP) uniform samplerCube u_cubemap;
layout (binding = TEXTURE_UNIT_DEPTH_MAP) uniform sampler2D u_depthMap;

void main()
{
    gPosition = v_out.WorldPosition;
    gNormal = v_out.Normal;
    gAlbedo = texture(u_normal, v_out.UV);
}