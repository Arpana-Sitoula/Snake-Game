#version 460 core
layout(location = 2) in vec2 in_uv;
layout(location = 0) out vec4 out_color;

layout(binding = 0) uniform sampler2D tex_diffuse;

void main() {
    vec2 texSize = textureSize(tex_diffuse, 0);
    vec2 texelSize = 1.0 / texSize;
    
    vec4 color = vec4(0.0);
    
    // A simple 5x5 box blur
    const float radius = 2.0; 
    float weightSum = 0.0;
    
    for(float x = -radius; x <= radius; x += 1.0) {
        for(float y = -radius; y <= radius; y += 1.0) {
            color += texture(tex_diffuse, in_uv + vec2(x, y) * texelSize);
            weightSum += 1.0;
        }
    }
    
    out_color = color / weightSum;
}
