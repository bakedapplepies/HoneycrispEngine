#version 450 core 
out vec4 FragColor;  // Color output


// Export Vertex Attributes from previous shader stage
in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
    vec4 FragPosDepthSpace;
} fs_in;

uniform vec3 u_color;

void main()
{
    FragColor = vec4(u_color, 1.0);
}