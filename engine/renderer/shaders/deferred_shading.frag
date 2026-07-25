#version 460 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 bloomThresColor;

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

const uint UBO_BINDING_GLOBAL      = 0;
const uint UBO_BINDING_LIGHT       = 1;
const uint UBO_BINDING_MATERIAL    = 2;
const uint UBO_BINDING_POSTPROCESS = 3;

const uint TEXTURE_UNIT_ALBEDO    = 0;
const uint TEXTURE_UNIT_NORMAL    = 1;
const uint TEXTURE_UNIT_ROUGHNESS = 2;
const uint TEXTURE_UNIT_CUBEMAP   = 3;
const uint TEXTURE_UNIT_DEPTH_MAP = 4;

const uint TEXTURE_UNIT_GBUFFER_POSITION  = 0;
const uint TEXTURE_UNIT_GBUFFER_NORMAL    = 1;
const uint TEXTURE_UNIT_GBUFFER_ROUGHNESS = 2;
const uint TEXTURE_UNIT_GBUFFER_ALBEDO    = 3;

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
    vec2 UV;
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

layout (binding = UBO_BINDING_POSTPROCESS, std140) uniform PostprocessUBO
{
    float u_exposure;
    float u_bloomThres;
    float u_bloomFilterRad;
};

// Textures
layout (binding = TEXTURE_UNIT_GBUFFER_POSITION) uniform sampler2D u_worldPosition;
layout (binding = TEXTURE_UNIT_GBUFFER_ALBEDO) uniform sampler2D u_albedo;
layout (binding = TEXTURE_UNIT_GBUFFER_NORMAL) uniform sampler2D u_normal;
layout (binding = TEXTURE_UNIT_GBUFFER_ROUGHNESS) uniform sampler2D u_roughness;
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

// Normal distribution function for the Microfacet model
// Trowbridge-Reitz GGX
float D_GGX(float alpha, float cos_theta_h)
{
    float alpha_sqrd = pow(alpha, 2);
    float cos_theta_h2 = pow(cos_theta_h, 2);
    return alpha_sqrd / (PI * pow((alpha_sqrd - 1.0) * cos_theta_h2 + 1.0, 2));
}

float Lambda_GGX(float a)
{
    return (-1.0 + sqrt(1.0 + 1.0/(a*a))) * 0.5;
}

float G_Smith(float alpha, vec3 H, vec3 S)
{
    float HdotS = dot_clamp01(H, S);
    float a = HdotS / (max(alpha, 0.0001) * sqrt(1.0 - HdotS*HdotS));
    return 1.0 / (1.0 + Lambda_GGX(a));
}

// Geometric attenuation factor for the Microfacet model
float G(float alpha, vec3 w_i, vec3 w_o, vec3 H)
{
    return 1.0 / (1.0 + G_Smith(alpha, H, w_i) + G_Smith(alpha, H, w_o));
}

float G_SchlickGXX(vec3 w_i, vec3 N, float k)
{
    float NdotV = dot_clamp01(w_i, N);
    return NdotV / (NdotV * (1.0 - k) + k);
}

// Another geometric attenuation factor for the Microfacet model
float G2(vec3 w_i, vec3 w_o, vec3 N, float alpha)
{
    float k = pow(alpha + 1, 2) / 8.0;
    float ggx1 = G_SchlickGXX(w_i, N, k);
    float ggx2 = G_SchlickGXX(w_o, N, k);

    return ggx1 * ggx2;
}

// Fresnel factor for the Microfacet model
float F_Schlick(vec3 w_o, vec3 N, float F0)
{
    float cos_theta_v = dot_clamp01(N, w_o);
    float F90 = 1.0;
    return F0 + (F90 - F0) * pow(1.0 - cos_theta_v, 5.0);
}

vec3 Lambertian_DiffuseBRDF(vec3 w_i, vec3 w_o)
{
    return vec3(1.0);
}

vec3 Burley_DiffuseBRDF(vec3 w_i, vec3 w_o, vec3 N, vec3 H, float roughness)
{
    float F_D90 = 0.5 + 2 * roughness * pow(dot_clamp01(w_i, H), 2);
    float cos_theta_l = dot_clamp01(w_i, N);
    float cos_theta_v = dot_clamp01(w_o, N);

    return vec3(1.0 + (F_D90 - 1.0)*pow(1.0 - cos_theta_l, 5)) * (1.0 + (F_D90 - 1.0)*pow(1.0 - cos_theta_v, 5)) / PI;
}

vec3 BlinnPhong_SpecularBRDF(vec3 N, vec3 H, float shininess)
{
    return vec3(pow(dot_clamp01(N, H), shininess));
}

vec3 Microfacet_SpecularBRDF(vec3 w_i, vec3 w_o, vec3 N, vec3 H, float roughness, inout float Fr)
{
    float alpha = pow(roughness, 2);
    float cos_theta_h = dot_clamp01(N, H);
    float D = D_GGX(alpha, cos_theta_h);
    // float G = G(alpha, w_i, w_o, H);
    float G2 = G2(w_i, w_o, N, alpha);
    float F = F_Schlick(w_o, N, 0.04);
    Fr = F;

    return vec3((D * G2 * F) / max(4 * dot_clamp01(N, w_i) * dot_clamp01(N, w_o), 0.001));
    // return vec3(F);
}

vec3 DiffuseBRDF(float k_diffuse, float roughness, vec3 w_i, vec3 w_o, vec3 N, vec3 H)
{
#if LAMBERTIAN_DIFFUSE
    return k_diffuse * Lambertian_DiffuseBRDF(w_i, w_o);
#elif BURLEY_DIFFUSE
    return k_diffuse * Burley_DiffuseBRDF(w_i, w_o, N, H, roughness);
#else
    return k_diffuse * Lambertian_DiffuseBRDF(w_i, w_o);
#endif
}

vec3 SpecularBRDF(float k_specular, float roughness, float shininess, vec3 w_i, vec3 w_o, vec3 N, vec3 H, inout float Fr)
{
#if USE_SPECULAR
    #if BLINN_PHONG_SPECULAR
        return k_specular * BlinnPhong_SpecularBRDF(N, H, shininess);
    #elif MICROFACET_SPECULAR
        return k_specular * Microfacet_SpecularBRDF(w_i, w_o, N, H, roughness, Fr);
    #else
        return vec3(0.0);
    #endif
#else
    return vec3(0.0);
#endif
}

vec3 BRDF(float k_diffuse, float k_specular, float roughness, float shininess, vec3 w_i, vec3 w_o, vec3 N, vec3 H, vec3 albedo)
{
    float Fr = 0.0;
    vec3 brdf_diffuse = DiffuseBRDF(k_diffuse, roughness, w_i, w_o, N, H);
    vec3 brdf_specular = SpecularBRDF(k_specular, roughness, shininess, w_i, w_o, N, H, Fr);

#if BLEND_BRDF_FRESNEL
    return (1.0 - Fr) * brdf_diffuse + brdf_specular;
#else
    return albedo * brdf_diffuse + brdf_specular;
#endif
}

float ShadowModifier(vec4 light_space_pos)
{
    vec3 projCoords = light_space_pos.xyz / light_space_pos.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float shadow = 0.0;

    if (currentDepth > 1.0)
    {
        return shadow;
    }

    vec2 texelSize = 1.0 / textureSize(u_depthMap, 0);
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(u_depthMap, projCoords.xy + vec2(x, y) * texelSize).x;
            shadow += currentDepth > pcfDepth + 0.005 ? 1.0 : 0.0;
        }
    }

    return shadow / 9.0;
}

void main()
{
    vec3 color = vec3(0.0);

    vec3 worldPosition = texture(u_worldPosition, v_out.UV).xyz;
    vec3 normal = texture(u_normal, v_out.UV).xyz;
    vec3 albedo = texture(u_albedo, v_out.UV).xyz;
    vec3 roughness = texture(u_roughness, v_out.UV).xyz;

    float k_ambient = u_ambient_intensity;
    float k_diffuse = u_diffuse_intensity;
    // float k_diffuse = (1.0 - u_specular_intensity);
    float k_specular = u_specular_intensity;
    float k_roughness = u_roughness_scalar;
    float shininess = 32.0;
    float shadowMod = ShadowModifier(u_lightSpace * vec4(worldPosition, 1.0));
    vec3 emission = vec3(0.0, 0.0, 0.0);
    vec3 w_o = normalize(u_cameraPos.xyz - worldPosition);

    {   // Directional Light
        vec3 w_i = normalize(-u_dirLight.direction);
        vec3 H = normalize(w_i + w_o);
        
        vec3 brdf = BRDF(k_diffuse, k_specular, roughness.g, shininess, w_i, w_o, normal, H, albedo);
        float cos_theta_i = dot_clamp01(w_i, normal);
        float attenuation = inverse_square(length(u_dirLight.direction));
    
        color += (1.0 - shadowMod) * brdf * cos_theta_i * attenuation;
    }

    for (uint i = 0; i < u_numPointLight; i++)
    {
        vec3 w_i = normalize(u_pointLights[i].position - worldPosition);
        vec3 H = normalize(w_i + w_o);

        vec3 brdf = BRDF(k_diffuse, k_specular, roughness.g, shininess, w_i, w_o, normal, H, albedo);
        float cos_theta_i = dot_clamp01(w_i, normal);
        float attenuation = inverse_square(length(u_dirLight.direction));
    
        color += brdf * cos_theta_i * attenuation;
    }

    color += k_ambient * albedo;
    color += emission;

    fragColor = vec4(color, 1.0);

    float Lin = luminance(color);
    if (Lin >= u_bloomThres)
    {
        bloomThresColor = vec4(color, 1.0);
    }
    else
    {
        bloomThresColor = vec4(vec3(0.0), 1.0);
    }
}   