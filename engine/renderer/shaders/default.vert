#version 460 core

// Re-declare built-in variables for stricter interface
// due to more flexible pipelines being able to choose different
// shader programs.
out gl_PerVertex
{
    vec4 gl_Position;
    // float gl_PointSize;
    // float gl_ClipDistance[];
};

layout (location = 0) in vec3 i_Position;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_UV;
layout (location = 3) in vec3 i_inst_Translation;

layout (location = 0) out V_OUT
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 UV;
    vec4 LightSpacePosition;
    flat uint instanceID;
} v_out;

layout (binding = 0, std140) uniform GlobalUBO
{
    float u_time;
    mat4 u_view;
    mat4 u_projection;
    vec4 u_cameraPos;
    mat4 u_lightSpace;
};

uniform mat4 u_model;
uniform mat3 u_normalMat;
uniform bool u_isEmissive;

const vec3 COLORS[4] = {
    vec3(0.48627, 0.80392, 0.53725),
    vec3(0.99607, 0.96078, 0.69411),
    vec3(0.82745, 0.72549, 0.62352),
    vec3(0.98823, 0.47058, 0.46274)
};

void main()
{
    // vec4 worldPosition = u_model * vec4(i_Position, 1.0);
    vec4 worldPosition = u_model * vec4(i_Position + i_inst_Translation, 1.0);
    v_out.WorldPosition = worldPosition.xyz;
    v_out.Normal = u_normalMat * i_Normal;  // if there are instanced rotations this would have to be changed
    v_out.UV = i_UV;
    v_out.LightSpacePosition = u_lightSpace * worldPosition;
    v_out.instanceID = gl_InstanceID;

    gl_Position = u_projection * u_view * worldPosition;
}