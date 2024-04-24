#version 460 core 
out vec4 FragColor;

precision mediump float;

in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} fs_in;

layout(std140, binding = 1) uniform GlobUniforms
{
    vec3 u_viewPos;
    vec3 spotLightPos;
    vec3 spotLightDir;
};

struct Material
{
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

uniform DirLight u_dirLight;
uniform PointLight u_pointLight;
uniform SpotLight u_spotLight;
uniform Material u_material;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 dirToView, vec3 albedoFrag, vec3 specularFrag)
{
    vec3 ambient = dirLight.ambient * albedoFrag;

    float diffuseCoef = max(dot(normal, -dirLight.direction), 0.0);
    vec3 diffuse = dirLight.diffuse * diffuseCoef * albedoFrag;

    vec3 reflectDir = reflect(dirLight.direction, normal);
    float specularCoef = pow(max(dot(reflectDir, dirToView), 0.0), u_material.shininess);
    vec3 specular = dirLight.specular * specularCoef * specularFrag;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 dirToView, vec3 albedoFrag, vec3 specularFrag)
{
    vec3 fragToLight = pointLight.position - fs_in.FragPos;
    vec3 dirToLight = normalize(fragToLight);
    vec3 halfwayVec = normalize(dirToLight + dirToView);

    // ambient - diffuse - specular
    vec3 ambient = pointLight.ambient * albedoFrag;

    float diffuseCoef = max(dot(normal, dirToLight), 0.0);
    vec3 diffuse = pointLight.diffuse * diffuseCoef * albedoFrag;

    float specularCoef = pow(max(dot(normal, halfwayVec), 0.0), u_material.shininess);
    vec3 specular = pointLight.specular * specularCoef * specularFrag;
    
    // attenuation
    float dist = length(fragToLight);
    float attenuation = 1 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * dist * dist);
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 dirToView, vec3 albedoFrag, vec3 specularFrag)
{
    vec3 fragToLight = u_viewPos - fs_in.FragPos;
    vec3 dirToLight = normalize(fragToLight);

    // light boundary
    float theta = dot(dirToLight, normalize(-spotLightDir));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    // ambient - diffuse - specular
    vec3 ambient = spotLight.ambient * albedoFrag;

    float diffuseCoef = max(dot(normal, dirToLight), 0.0);
    vec3 diffuse = spotLight.diffuse * diffuseCoef * albedoFrag;

    vec3 reflectDir = reflect(-dirToLight, normal);
    float specularCoef = pow(max(dot(reflectDir, dirToView), 0.0), u_material.shininess);
    vec3 specular = spotLight.specular * specularCoef * specularFrag;

    // attenuation
    float dist = length(fragToLight);
    float attenuation = 1 / (spotLight.constant + spotLight.linear * dist + spotLight.quadratic * dist * dist);

    return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main()
{
    vec3 result = vec3(0.0);
    vec3 fragToView = u_viewPos - fs_in.FragPos;
    vec3 dirToView = normalize(fragToView);
    vec3 albedoFrag = vec3(texture(u_material.albedo, fs_in.TexCoord));
    vec3 specularFrag = vec3(texture(u_material.specular, fs_in.TexCoord));

    // result += CalcDirLight(u_dirLight, fs_in.Normal, dirToView, albedoFrag, specularFrag);
    // if (u_material.normal)
    // {
    // result += CalcPointLight(u_pointLight, vec3(texture(u_material.normal, fs_in.TexCoord)), dirToView, albedoFrag, specularFrag);
    // }
    // else
    // {
        result += CalcPointLight(u_pointLight, fs_in.Normal, dirToView, albedoFrag, specularFrag);
    // }
    // result += CalcSpotLight(u_spotLight, fs_in.Normal, dirToView, albedoFrag, specularFrag);

    FragColor = vec4(result, 1.0);
}