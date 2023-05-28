#version 400 
out vec4 FragColor;

precision mediump float;

in vec3 VertColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_light;
uniform Material u_material;


void main()
{
    // ambient
    vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, TexCoord));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, TexCoord));

    // specular
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, TexCoord));

    vec3 result = ambient + diffuse + specular;

    FragColor = texture(u_Texture, TexCoord) * vec4(result, 1.0) * vec4(VertColor, 1.0);
}