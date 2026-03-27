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
layout(location = 19) uniform int u_is_emissive;

// 4 Point Lights
layout(location = 20) uniform vec3 u_light_pos[4];
layout(location = 30) uniform vec3 u_light_col[4];
layout(location = 40) uniform int u_use_lighting;

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

    // EMISSIVE BYPASS (Static glow)
    if (u_is_emissive != 0) {
        out_color = base_col;
        return;
    }

    // 3. LIGHTING
    if (u_use_lighting == 0) {
        // FULL BRIGHT (For UI/Snake Game)
        out_color = base_col;
        return;
    }

    vec3 total_diffuse = vec3(0.0);
    float ambient_str = 0.03; // Dark Effect
    vec3 ambient_light = vec3(1.0, 0.95, 0.9) * ambient_str;

    for (int i = 0; i < 4; i++) {
        vec3 light_dir = normalize(u_light_pos[i] - in_pos);
        float dist = length(u_light_pos[i] - in_pos);
        
        // Attenuation (Light drops off with distance)
        float attenuation = 1.0 / (1.0 + 0.1 * dist + 0.02 * (dist * dist));
        
        float diffuse_fact = calc_diffuse(normal, light_dir);
        total_diffuse += u_light_col[i] * diffuse_fact * attenuation;
    }

    // 4. FINAL OUTPUT
    out_color.xyz = (ambient_light + total_diffuse) * base_col.xyz;
    out_color.a = base_col.a;
}