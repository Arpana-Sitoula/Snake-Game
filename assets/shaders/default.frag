#version 460 core

// input (from vertex shader)
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_col;

// output (to pixel of output image)
layout(location = 0) out vec4 out_color;

// texture unit
layout(binding = 0) uniform sampler2D tex_diffuse;

// uniform color override
layout(location = 16) uniform vec4 u_color;
layout(location = 17) uniform int u_use_uniform_color;
layout(location = 18) uniform int u_use_texture;

// calculate the diffuse light strength
float calc_diffuse(vec3 normal, vec3 light_dir) {
    float diffuse = dot(normal, light_dir);
    return max(diffuse, 0.0);
}

void main() {
    vec3 normal = normalize(in_norm);
    
    // 1. BASE COLOR (Uniform or Vertex)
    vec4 base_col = (u_use_uniform_color != 0) ? u_color : in_col;
    
    // 2. TEXTURE SAMPLING
    if (u_use_texture != 0) {
        vec4 tex_col = texture(tex_diffuse, in_uv);
        base_col = tex_col;
    }

    // 3. LIGHTING
    vec3 light_col = vec3(1.0, 0.95, 0.85); // warm light
    vec3 light_pos = vec3(5, 10, 5); 
    vec3 light_dir = normalize(light_pos - in_pos);

    // ambient light
    float ambient_str = 0.4; 
    vec3 ambient_light = light_col * ambient_str;
    
    // diffuse light
    float diffuse_str = calc_diffuse(normal, light_dir);
    vec3 diffuse_light = light_col * diffuse_str;

    // 4. FINAL OUTPUT
    out_color.xyz = (ambient_light + diffuse_light) * base_col.xyz;
    out_color.a = base_col.a;
}