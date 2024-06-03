#version 460 core 
out vec4 FragColor;  // Color output



// Export Vertex Attributes from previous shader stage
in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
    vec4 FragPosDepthSpace;
} fs_in;

// layout (location = 0) uniform sampler2D

void main()
{
    
}