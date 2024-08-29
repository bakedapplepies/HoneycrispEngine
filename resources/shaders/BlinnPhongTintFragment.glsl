#version 460 core 
out vec4 FragColor;


in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
    vec4 FragPosDepthSpace;
} fs_in;

layout(std140, binding = 1) uniform GlobUniforms
{
    vec3 u_viewPos;
    vec3 u_spotLightPos;
    vec3 u_spotLightDir;
};

struct Material
{
    uint whichMaterial;
    sampler2D albedo;
    sampler2D roughness;
    sampler2D ao;
    sampler2D normal;
    sampler2D specular;

    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int u_num_dir_light = 0;
uniform int u_num_point_light = 1;
uniform int u_num_spot_light = 0;
uniform sampler2D u_framebuffer_depth_texture;
uniform DirLight u_dir_light;
uniform PointLight u_point_light;
uniform SpotLight u_spot_light;
uniform Material u_material;

float ShadowFactor(vec4 frag_pos_depth_space)
{
    // perspective divide
    vec3 ndc = frag_pos_depth_space.xyz / frag_pos_depth_space.w;
    vec2 uv = ndc.xy * 0.5 + 0.5;

    float closestDepth = texture(u_framebuffer_depth_texture, uv).r;
    float currentDepth = ndc.z;
    float shadow = currentDepth > closestDepth ? 0.0 : 1.0;

    return shadow;
}

vec3 CalcDirLight(DirLight dir_light, vec3 normal, vec3 dir_to_view, float shadow_factor, vec3 albedo_frag, vec3 specular_frag)
{
    vec3 ambient = dir_light.ambient * albedo_frag;

    float diffuseCoef = max(dot(normal, -dir_light.direction), 0.0);
    vec3 diffuse = dir_light.diffuse * diffuseCoef * albedo_frag;

    vec3 reflectDir = reflect(dir_light.direction, normal);
    float specularCoef = pow(max(dot(reflectDir, dir_to_view), 0.0), u_material.shininess);
    vec3 specular = dir_light.specular * specularCoef * specular_frag;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight point_light, vec3 normal, vec3 dir_to_view, vec3 albedo_frag, vec3 specular_frag)
{
    vec3 fragToLight = point_light.position - fs_in.FragPos;
    vec3 dirToLight = normalize(fragToLight);
    vec3 halfwayVec = normalize(dirToLight + dir_to_view);

    // ambient
    vec3 ambient = point_light.ambient * albedo_frag;

    // diffuse
    float diffuseCoef = max(dot(normal, dirToLight), 0.0);
    vec3 diffuse = point_light.diffuse * diffuseCoef * albedo_frag;

    // specular
    vec3 reflectDir = reflect(-dirToLight, normal);
    float specularCoef = pow(max(dot(normal, halfwayVec), 0.0), u_material.shininess);
    vec3 specular = specular_frag * specularCoef;
    
    // attenuation
    float dist = length(fragToLight);

    // This is still useful for fine-tuning attenuation
    // float attenuation = 1 / (point_light.constant + point_light.linear * dist + point_light.quadratic * dist * dist);
    float attenuation = 1.0 / (dist * dist + 0.01);
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight spot_light, vec3 normal, vec3 dir_to_view, vec3 albedo_frag, vec3 specular_frag)
{
    vec3 fragToLight = u_viewPos - fs_in.FragPos;
    vec3 dirToLight = normalize(fragToLight);
    vec3 halfwayVec = normalize(dirToLight + dir_to_view);

    // light boundary
    float theta = dot(dirToLight, normalize(-u_spotLightDir));
    float epsilon = spot_light.cutOff - spot_light.outerCutOff;
    float intensity = clamp((theta - spot_light.outerCutOff) / epsilon, 0.0, 1.0);

    // ambient - diffuse - specular
    vec3 ambient = spot_light.ambient * albedo_frag;

    float diffuseCoef = max(dot(normal, dirToLight), 0.0);
    vec3 diffuse = spot_light.diffuse * diffuseCoef * albedo_frag;

    float specularCoef = pow(max(dot(normal, halfwayVec), 0.0), u_material.shininess);
    vec3 specular = spot_light.specular * specularCoef * specular_frag;

    // attenuation
    float dist = length(fragToLight);
    // float attenuation = 1 / (spot_light.constant + spot_light.linear * dist + spot_light.quadratic * dist * dist);
    float attenuation = 1 / (dist * dist);

    return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main()
{
    vec3 result = vec3(0.0);
    vec3 fragToView = u_viewPos - fs_in.FragPos;
    vec3 dirToView = normalize(fragToView);

    // Textures
    vec4 albedoFrag = texture(u_material.albedo, fs_in.TexCoord);
    vec4 specularFrag = vec4(0.0);
    if ((u_material.whichMaterial & (1 << 4)) == 1 << 4)
    {
        vec4 specularFrag = texture(u_material.specular, fs_in.TexCoord);
    }

    // Shadow space calculations
    float shadowFactor = ShadowFactor(fs_in.FragPosDepthSpace);

    // Accumulate light
    for (int i = 0; i < u_num_dir_light; i++)
        result += CalcDirLight(u_dir_light, fs_in.Normal, dirToView, shadowFactor, vec3(albedoFrag), vec3(specularFrag));

    for (int i = 0; i < u_num_point_light; i++)
        result += CalcPointLight(u_point_light, fs_in.Normal, dirToView, vec3(albedoFrag), vec3(specularFrag));
        
    for (int i = 0; i < u_num_spot_light; i++)
        result += CalcSpotLight(u_spot_light, fs_in.Normal, dirToView, vec3(albedoFrag), vec3(specularFrag));

    // No clamping since using HDR
    // result = clamp(result, vec3(0.0), vec3(1.0));

    FragColor = vec4(fs_in.VertColor, 1.0) * vec4(result, albedoFrag.w);
}