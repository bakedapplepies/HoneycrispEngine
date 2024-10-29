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
uniform float u_contrast = 1.0;
uniform float u_saturation = 1.0;
uniform float u_postExposure = 1.0;
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

vec3 color_grade_post_exposure(vec3 color)
{
    return color * u_postExposure;
}

vec3 color_grade_contrast(vec3 color)
{
    return (color - ACEScc_MIDGRAY) * u_contrast + ACEScc_MIDGRAY;
}

vec3 color_grade_color_filter(vec3 color)
{
    return color * u_colorFilter;
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
    vec2 uv = fs_in.UV;
    color = vec3(texture(u_framebuffer_color_texture, uv));

    // Color Correction
    color = color_grade_post_exposure(color);
    color = color_grade_contrast(color);
    color = max(color, vec3(0.0));
    // color = u_contrast * (color - vec3(0.5)) + vec3(0.5) + vec3(u_brightness);
    color = lerp(luminance(color).xxx, color, u_saturation);
    color = color_grade_color_filter(color);

    // Extended Reinhard Tone mapping
    color = extended_reinhard(color, u_cwhite);
    
    // Gamma Correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}