#version 460 core 
out vec4 FragColor;  // Color output

precision mediump float;

// Export Vertex Attributes from previous shader stage
in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} fs_in;

void main()
{
    
}