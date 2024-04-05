#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} gs_in[];

out VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} gs_out;

layout (std140, binding = 0) uniform Matrices
{ 
    mat4 u_view;
    mat4 u_projection;
    float u_time;
};

uniform float u_normal_length;

void UpdateVertAttribs(int index)
{
    gs_out.VertColor = gs_in[index].VertColor;
    gs_out.TexCoord = gs_in[index].TexCoord;
    gs_out.Normal = gs_in[index].Normal;
    gs_out.FragPos = gs_in[index].FragPos;
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

    return normalize(cross(a, b));
}

void GenerateLine(int index, vec3 normal)
{
    gl_Position = u_projection * gl_in[index].gl_Position;
    UpdateVertAttribs(index);
    EmitVertex();

    gl_Position = u_projection * (gl_in[index].gl_Position + vec4(normal, 0.0) * u_normal_length);
    UpdateVertAttribs(index);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 normal = GetNormal();
    GenerateLine(0, normal);
    GenerateLine(1, normal);
    GenerateLine(2, normal);
}