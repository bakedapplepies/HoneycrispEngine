#version 460 core

out vec4 FragColor;

#define LAMBERTIAN_DIFFUSE   0
#define BURLEY_DIFFUSE       1
#define BLINN_PHONG_SPECULAR 0
#define MICROFACET_SPECULAR  1
#define USE_SPECULAR         0
#define BLEND_BRDF_FRESNEL   0

// Constants
const float PI = 3.1415;

const uint UBO_BINDING_GLOBAL   = 0;
const uint UBO_BINDING_LIGHT    = 1;
const uint UBO_BINDING_MATERIAL = 2;

const uint TEXTURE_UNIT_0       = 0;
const uint TEXTURE_UNIT_1       = 1;
const uint TEXTURE_UNIT_2       = 2;
const uint TEXTURE_UNIT_CUBEMAP = 3;

const uint MAX_POINT_LIGHT = 20;

// Structs
struct PointLight
{
    vec3 position;
    // float radius_thres;
};

// Data from Vertex stage
layout (location = 0) in V_OUT
{
    vec3 o_WorldPosition;
    vec3 o_Normal;
    vec2 o_UV;
    flat uint instanceID;
} v_out;

// UBOs
layout (binding = UBO_BINDING_GLOBAL, std140) uniform GlobalUBO
{
    float u_time;
    mat4 u_view;
    mat4 u_projection;
    vec4 u_cameraPos;
};

layout (binding = UBO_BINDING_LIGHT, std140) uniform LightUBO
{
    uint u_numPointLight;
    PointLight u_pointLights[MAX_POINT_LIGHT];
};

layout (binding = UBO_BINDING_MATERIAL, std140) uniform MaterialUBO
{
    float u_ambient;
    float u_diffuse;
    float u_k_roughness;
};

// Textures
layout (binding = TEXTURE_UNIT_0) uniform sampler2D u_albedo;
layout (binding = TEXTURE_UNIT_1) uniform sampler2D u_normal;
layout (binding = TEXTURE_UNIT_2) uniform sampler2D u_roughness;

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
float D_GGX(float alpha, float cos_theta_h)
{
    float alpha_sqrd = pow(alpha, 2);
    return alpha_sqrd / (PI * pow((alpha_sqrd - 1.0) * cos_theta_h + 1.0, 2));
}

float Lambda_GGX(float a)
{
    return (-1 + sqrt(1 + 1/(a*a))) / 2;
}

float G_Smith(float alpha, vec3 H, vec3 S)
{
    float HdotS = dot_clamp01(H, S);
    float a = HdotS / (alpha * sqrt(1.0 - HdotS*HdotS));
    return 1.0 / (1.0 + Lambda_GGX(a));
}

// Geometric attenuation factor for the Microfacet model
float G(float alpha, vec3 w_i, vec3 w_o, vec3 H)
{
    return 1.0 / (1.0 + G_Smith(alpha, H, w_i) + G_Smith(alpha, H, w_o));
}

// Fresnel factor for the Microfacet model
float F_Schlick(vec3 w_o, vec3 N, float F0)
{
    float cos_theta_v = dot_clamp01(N, w_o);
    float F90 = 1.0;
    return F0 + (F90 - F0) * (1 - pow(cos_theta_v, 5));
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

vec3 Microfacet_SpecularBRDF(vec3 w_i, vec3 w_o, vec3 N, vec3 H, float roughness)
{
    float alpha = pow(roughness, 2);
    float cos_theta_h = dot_clamp01(N, H);
    float D = D_GGX(alpha, cos_theta_h);
    float G = G(alpha, w_i, w_o, H);
    float F = F_Schlick(w_o, N, 0.04);

    return vec3((D * G * F) / (4 * dot_clamp01(N, w_i) * dot_clamp01(N, w_o)));
}

void main()
{
    float k_ambient = 0.01;
    float k_diffuse = 1.0;
    float k_specular = 0.2;
    float shininess = 32.0;
    vec3 albedoSample = texture(u_albedo, v_out.o_UV).xyz;
    vec3 roughnessSample = texture(u_roughness, v_out.o_UV).xyz;

    vec3 w_i = normalize(u_pointLights[0].position - v_out.o_WorldPosition);
    vec3 w_o = normalize(u_cameraPos.xyz - v_out.o_WorldPosition);
    vec3 H = normalize(w_i + w_o);

    vec3 emission = vec3(0.0, 0.0, 0.0);

#if LAMBERTIAN_DIFFUSE
    vec3 brdf_diffuse = k_diffuse * Lambertian_DiffuseBRDF(w_i, w_o);
#elif BURLEY_DIFFUSE
    vec3 brdf_diffuse = k_diffuse * Burley_DiffuseBRDF(w_i, w_o, v_out.o_Normal, H, 0.0);
#else
    vec3 brdf_diffuse = k_diffuse * Lambertian_DiffuseBRDF(w_i, w_o);
#endif

#if USE_SPECULAR
#   if BLINN_PHONG_SPECULAR
        vec3 brdf_specular = k_specular * BlinnPhong_SpecularBRDF(v_out.o_Normal, H, shininess);
#   elif MICROFACET_SPECULAR
        vec3 brdf_specular = k_specular * Microfacet_SpecularBRDF(w_i, w_o, v_out.o_Normal, H, 0.5);
#   else
        vec3 brdf_specular = vec3(0.0);
#   endif
#else
    vec3 brdf_specular = vec3(0.0);
#endif

#if BLEND_BRDF_FRESNEL
    vec3 brdf = (1.0 - F) * brdf_diffse + brdf_specular;
#else
    vec3 brdf = albedoSample * brdf_diffuse + brdf_specular;
#endif

    float cos_theta_i = dot(w_i, v_out.o_Normal);
    
    vec3 color = brdf * cos_theta_i * inverse_square(length(u_cameraPos.xyz - v_out.o_WorldPosition));
    color += k_ambient * albedoSample;
    color += emission;

    color = clamp(color, vec3(0.0), vec3(1.0));

    FragColor = vec4(color, 1.0);
}   