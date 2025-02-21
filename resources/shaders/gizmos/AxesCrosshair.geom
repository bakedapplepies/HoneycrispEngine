#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

#define X_AXIS vec4(1.0, 0.0, 0.0, 0.0)
#define Y_AXIS vec4(0.0, 1.0, 0.0, 0.0)
#define Z_AXIS vec4(0.0, 0.0, 1.0, 0.0)

// Import Vertex Attributes from previous shader stage
in VS_OUT {
    vec3 VertColor;
} gs_in[];

// Export Vertex Attributes to next shader stage
out VS_OUT {
    vec3 VertColor;
} gs_out;

layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_ortho;
    mat4 u_projection;
    float u_time;
};

layout(std140, binding = 1) uniform GlobUniforms
{
    vec3 u_viewPos;
    vec3 spotLightPos;
    vec3 u_viewDir;
};

// Helper function to update exported vertex attributes
void UpdateVertAttribs(int index)
{
    gs_out.VertColor = gs_in[index].VertColor;
}

void main()
{
    // x-axis
    vec4 transform_x_axis = u_ortho * mat4(mat3(u_view)) * X_AXIS;
    gs_out.VertColor = vec3(1.0, 0.0, 0.0);
    gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = transform_x_axis;
    EmitVertex();
    EndPrimitive();

    // y-axis
    vec4 transform_y_axis = u_ortho * mat4(mat3(u_view)) * Y_AXIS;
    gs_out.VertColor = vec3(0.0, 1.0, 0.0);
    gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = transform_y_axis;
    EmitVertex();
    EndPrimitive();

    // z-axis
    vec4 transform_z_axis = u_ortho * mat4(mat3(u_view)) * Z_AXIS;
    gs_out.VertColor = vec3(0.0, 0.0, 1.0);
    gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = transform_z_axis;
    EmitVertex();
    EndPrimitive();
}