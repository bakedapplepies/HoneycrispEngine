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

struct Material
{
    sampler2D albedo;
    sampler2D roughness;
    sampler2D ao;
    sampler2D normal;
    sampler2D specular;

    float shininess;
};
uniform Material u_material;

void main()
{
    FragColor = texture(u_material.albedo, fs_in.TexCoord);
}