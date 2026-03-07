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

layout (location = 0) out V_OUT
{
    vec2 o_UV;
} v_out;

layout (binding = 0, std140) uniform GlobalUBO
{
    float u_time;
    mat4 u_view;
    mat4 u_projection;
    vec4 u_cameraPos;
};

void main()
{
    v_out.o_UV = i_Position.xy * 0.5 + vec2(0.5);

    gl_Position = vec4(i_Position, 1.0);
}