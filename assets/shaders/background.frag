#version 460 core
layout(location = 2) in vec2 in_uv;
layout(location = 0) out vec4 out_color;

layout(binding = 0) uniform sampler2D tex_diffuse;

void main() {
    out_color = texture(tex_diffuse, in_uv);
}
