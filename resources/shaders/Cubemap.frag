#version 450 core
out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube u_cubemap;

layout (std140, binding = 1) uniform GlobUniforms
{
    vec3 u_viewPos;
    vec3 spotLightPos;
    vec3 spotLightDir;
};

void main()
{
    FragColor = texture(u_cubemap, TexCoords);
}