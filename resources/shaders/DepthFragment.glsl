#version 460 core 
out vec4 FragColor;  // Color output

precision mediump float;

float near = 0.1;
float far = 100.0;

// Export Vertex Attributes from previous shader stage
in VS_OUT {
    vec3 VertColor;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} fs_in;

void main()
{
    float depth = gl_FragCoord.z;
    float ndc = depth * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near)) / far;
    FragColor = vec4(linearDepth);
}