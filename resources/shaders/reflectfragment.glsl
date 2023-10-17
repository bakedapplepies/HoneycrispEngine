#version 460 core 
out vec4 FragColor;

precision mediump float;

in vec3 VertColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

layout(std140, binding = 1) uniform GlobUniforms
{
    vec3 u_viewPos;
    vec3 spotLightPos;
    vec3 spotLightDir;
};

// struct Material
// {
//     sampler2D albedo;
//     sampler2D roughness;
//     sampler2D ao;
//     sampler2D normal;
//     sampler2D specular;

//     float shininess;
// };

// uniform Material u_material;
uniform samplerCube cubemap;


void main()
{
    vec3 viewToFrag = FragPos - u_viewPos;

    vec3 I = viewToFrag;
    vec3 R = reflect(I, Normal);

    FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}