#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(u_time) + 1) / 2) * magnitude;

    return position + vec4(direction, 0);
}

void main()
{
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.VertColor = gs_in[0].VertColor;
    gs_out.TexCoord = gs_in[0].TexCoord;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.FragPos = gs_in[0].FragPos;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.VertColor = gs_in[1].VertColor;
    gs_out.TexCoord = gs_in[1].TexCoord;
    gs_out.Normal = gs_in[1].Normal;
    gs_out.FragPos = gs_in[1].FragPos;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.VertColor = gs_in[2].VertColor;
    gs_out.TexCoord = gs_in[2].TexCoord;
    gs_out.Normal = gs_in[2].Normal;
    gs_out.FragPos = gs_in[2].FragPos;
    EmitVertex();

    EndPrimitive();
}