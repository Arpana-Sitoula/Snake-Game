#version 460 core
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_col;

layout(location = 2) out vec2 out_uv;

void main() {
    // Fill the screen - input quad is -0.5 to 0.5, so we scale it by 2 to fill -1 to 1 NDC
    gl_Position = vec4(in_pos.xy * 2.0, 0.9999, 1.0); 
    out_uv = in_uv;
}
