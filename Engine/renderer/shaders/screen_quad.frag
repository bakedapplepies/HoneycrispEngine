#version 460 core

out vec4 FragColor;

const uint TEXTURE_UNIT_0 = 0;
const uint TEXTURE_UNIT_1 = 1;
const uint TEXTURE_UNIT_2 = 2;
const uint TEXTURE_UNIT_CUBEMAP = 3;

layout (location = 0) in V_OUT
{
    vec2 o_UV;
} v_out;

layout (binding = TEXTURE_UNIT_0) uniform sampler2D u_texture;

void main()
{
    vec4 texSample = texture(u_texture, v_out.o_UV);

    texSample.xyz = pow(texSample.xyz, vec3(1.0/2.2));

    FragColor = texSample;
}   