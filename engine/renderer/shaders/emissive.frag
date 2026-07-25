#version 460 core

layout (location = 0) out vec4 fragColor;

#define LAMBERTIAN_DIFFUSE   0
#define BURLEY_DIFFUSE       1
#define BLINN_PHONG_SPECULAR 0
#define MICROFACET_SPECULAR  1
#define USE_SPECULAR         1
#define BLEND_BRDF_FRESNEL   0

#if LAMBERTIAN_DIFFUSE && BURLEY_DIFFUSE
    #error Only one Diffuse BRDF can be chosen.
#endif
#if BLINN_PHONG_SPECULAR && MICROFACET_SPECULAR
    #error Only one Specular BRDF can be chosen.
#endif

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

const vec3 COLORS[4] = {
    vec3(0.48627, 0.80392, 0.53725),
    vec3(0.99607, 0.96078, 0.69411),
    vec3(0.82745, 0.72549, 0.62352),
    vec3(0.98823, 0.47058, 0.46274)
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

// Textures
layout (binding = TEXTURE_UNIT_ALBEDO) uniform sampler2D u_albedo;
layout (binding = TEXTURE_UNIT_NORMAL) uniform sampler2D u_normal;
layout (binding = TEXTURE_UNIT_ROUGHNESS) uniform sampler2D u_roughness;
layout (binding = TEXTURE_UNIT_CUBEMAP) uniform samplerCube u_cubemap;
layout (binding = TEXTURE_UNIT_DEPTH_MAP) uniform sampler2D u_depthMap;

// Functions
float luminance(vec3 color)
{
    vec3 weights = vec3(0.2126, 0.7152, 0.0722);
    return dot(weights, color);
}

float inverse_square(float r)
{
    return 1.0/(1.0 + pow(r, 2));
}

float dot_clamp01(vec3 a, vec3 b)
{
    return clamp(dot(a, b), 0.0, 1.0);
}

void main()
{
    uint v = uint(v_out.WorldPosition.x) * uint(v_out.WorldPosition.y) * uint(v_out.WorldPosition.z) * 43758 % 4;
    vec3 color = COLORS[v];

    fragColor = vec4(color, 1.0);
}