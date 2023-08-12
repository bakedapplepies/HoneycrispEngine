#version 400 
out vec4 FragColor;

precision mediump float;

in vec3 VertColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_viewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight u_light;
uniform Material u_material;


void main()
{
    vec3 lightDir = normalize(u_light.position - FragPos);  // frag to lightPos

    float theta = dot(lightDir, normalize(-u_light.direction));
    float epsilon = u_light.cutOff - u_light.outerCutOff;
    float intensity = clamp((theta - u_light.outerCutOff) / epsilon, 0.0, 1.0);

    // ambient
    vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, TexCoord));

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, TexCoord));

    // specular
    vec3 viewDir = normalize(u_viewPos - FragPos);  // frag to view
    vec3 reflectDir = reflect(-lightDir, norm);  // points in general direction of viewDir
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, TexCoord));

    // attenuation
    float dist = length(u_light.position - FragPos);
    float attenuation = 1.0 / (u_light.constant + u_light.linear * dist + u_light.quadratic * dist * dist);

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(VertColor, 1.0) * vec4(result, 1.0);
}