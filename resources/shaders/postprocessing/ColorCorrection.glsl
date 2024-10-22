#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec2 UV;
} fs_in;

#define ACEScc_MIDGRAY 0.4135884

uniform vec2     u_viewport_size;
uniform sampler2D u_framebuffer_color_texture;
uniform sampler2D u_framebuffer_depth_texture;

uniform float u_brightness = 0.0;
uniform float u_contrast = 0.0;
uniform float u_saturation = 1.0;
uniform float u_postExposure = 0.0;
uniform float u_temperature;
uniform float u_tint;
uniform float u_cwhite = 60.0;
uniform vec3  u_colorFilter = vec3(1.0);

vec3 lerp(vec3 v1, vec3 v2, float interpolator)
{
    return (1.0 - interpolator) * v1 + interpolator * v2;
}

float luminance(vec3 v)
{
    return dot(v, vec3(0.2126, 0.7152, 0.0722));
}

vec3 change_luminance(vec3 color, float l_out)
{
    float l_in = luminance(color);
    return color * (l_out / l_in);
}

vec3 extended_reinhard(vec3 color, float max_white)
{
    float l_old = luminance(color);
    float numerator = l_old * (1.0 + (l_old / pow(max_white, 2)));
    float l_new = numerator / (1.0 + l_old);
    return change_luminance(color, l_new);
}

void main()
{   
    vec3 color = vec3(0.0);
    // vec2 uv = fs_in.UV;
    // vec2 ndc = (uv - 0.5) * 2.0;
    // ndc.x *= u_viewport_size.x / u_viewport_size.y;
    // vec2 center = vec2(0.0, 0.0);
    // float d = length(ndc - center);
    color = vec3(texture(u_framebuffer_color_texture, fs_in.UV));

    // Color Correction
    color *= u_postExposure;
    color = (color - ACEScc_MIDGRAY) * u_contrast + ACEScc_MIDGRAY;
    // color = u_contrast * (color - vec3(0.5)) + vec3(0.5) + vec3(u_brightness);
    color = lerp(luminance(color).xxx, color, u_saturation);
    color *= u_colorFilter;

    // Extended Reinhard Tone mapping
    color = extended_reinhard(color, u_cwhite);
    
    // Gamma Correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}