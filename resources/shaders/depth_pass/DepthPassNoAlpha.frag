#version 450 core

in VS_OUT {
    vec2 TexCoord;
} fs_in;

struct Material
{
    sampler2D albedo;

    float shininess;
};
uniform Material u_material;

void main()
{
    vec4 albedoFrag = texture(u_material.albedo, fs_in.TexCoord);
    if (albedoFrag.w < 1.0)
    {
        gl_FragDepth = 0.0;
    }
    else
    {
        gl_FragDepth = gl_FragCoord.z;
    }
    // This is what is done behind the scenes by OpenGL.
}