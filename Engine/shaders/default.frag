#version 460 core

out vec4 FragColor;

const uint TEXTURE_UNIT_0 = 0;
const uint TEXTURE_UNIT_1 = 1;
const uint TEXTURE_UNIT_2 = 2;
const uint TEXTURE_UNIT_CUBEMAP = 3;

layout (location = 0) in V_OUT
{
    vec3 o_WorldPosition;
    vec3 o_Normal;
    vec2 o_UV;
    flat uint instanceID;
} v_out;

layout (binding = TEXTURE_UNIT_0) uniform sampler2D u_albedo;

void main()
{
    vec3 albedoSample = texture(u_albedo, v_out.o_UV).xyz;
    // vec3 albedoSample = vec3(v_out.instanceID / 10000.0);

    FragColor = vec4(albedoSample, 1.0);
}   