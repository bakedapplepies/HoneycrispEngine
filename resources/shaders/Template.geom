#version 450 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Import Vertex Attributes from previous shader stage
in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} gs_in[];

// Export Vertex Attributes to next shader stage
out VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} gs_out;

// Global Uniforms
layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_ortho;
    mat4 u_projection;
    float u_time;
};

// Helper function to update exported vertex attributes
void UpdateVertAttribs(int index)
{
    gs_out.VertColor = gs_in[index].VertColor;
    gs_out.TexCoord = gs_in[index].TexCoord;
    gs_out.Normal = gs_in[index].Normal;
    gs_out.FragPos = gs_in[index].FragPos;
}

void main()
{
    
}