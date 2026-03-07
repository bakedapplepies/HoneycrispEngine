#version 450 core 
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

uniform samplerCube cubemap;


void main()
{
    vec3 viewToFrag = fs_in.FragPos - u_viewPos;

    vec3 I = viewToFrag;
    vec3 R = reflect(I, fs_in.Normal);

    FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}