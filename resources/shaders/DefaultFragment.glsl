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
    vec3 spotLightPos;
    vec3 spotLightDir;
};

void main()
{
    FragColor = vec4(fs_in.VertColor, 1.0);
}