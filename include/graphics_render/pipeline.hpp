#pragma once

/**
 * PIPELINE / SHADER (Graphics Render)
 * Purpose: Handles the tiny programs (Shaders) that run directly on the GPU.
 * 
 * Flow:
 * - init(): Loads two text files (`.vert` and `.frag`) from disk, compiles them, 
 *   and links them together into a "Graphics Pipeline" (Program)
 * - bind(): Tells the GPU to use this specific program for the next draw calls
 * - set_color(): Sends a custom RGB color to the Shader (overriding whatever color the mesh had)
 */
struct Pipeline {
    void init(const std::string& vs_name, const std::string& fs_name) {
        // base directory of the executable
        std::string base_path = SDL_GetBasePath();

        // load vertex shader from disk
        std::ifstream vs_stream{ base_path + "assets/shaders/" + vs_name };
        if (!vs_stream.is_open()) {
            std::println("Could not find {}", vs_name);
            std::exit(1);
        }
        std::string vs_string{ std::istreambuf_iterator<char>{ vs_stream }, {} };

        // load fragment shader from disk
        std::ifstream fs_stream{ base_path + "assets/shaders/" + fs_name };
        if (!fs_stream.is_open()) {
            std::println("Could not find {}", fs_name);
            std::exit(1);
        }
        std::string fs_string{ std::istreambuf_iterator<char>{ fs_stream }, {} };

        // prep the debug vars
        GLint success;
        std::vector<GLchar> info_log;
        info_log.resize(512);

        // compile vertex shader
        GLuint shader_vertex = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vs_string_glchar = vs_string.data();
        glShaderSource(shader_vertex, 1, &vs_string_glchar, nullptr);
        glCompileShader(shader_vertex);
        // check compilation results
        glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader_vertex, info_log.size(), nullptr, info_log.data());
            std::println("{} compilation error:\n {}", vs_name, info_log.data());
            std::exit(1);
        }

        // compile fragment shader (pixel shader)
        GLuint shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fs_string_glchar = fs_string.data();
        glShaderSource(shader_fragment, 1, &fs_string_glchar, nullptr);
        glCompileShader(shader_fragment);
        // check compilation results
        glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader_fragment, info_log.size(), nullptr, info_log.data());
            std::println("{} compilation error:\n {}", fs_name, info_log.data());
            std::exit(1);
        }

        // create graphics pipeline (aka program)
        _shader_program = glCreateProgram();
        glAttachShader(_shader_program, shader_vertex);
        glAttachShader(_shader_program, shader_fragment);
        glLinkProgram(_shader_program);
        // check linking results
        glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(_shader_program, info_log.size(), nullptr, info_log.data());
            std::println("{} and {} linking error:\n {}", vs_name, fs_name, info_log.data());
            std::exit(1);
        }

        // clean up shader sources after compilation
        glDeleteShader(shader_vertex);
        glDeleteShader(shader_fragment);
    }
    void destroy() {
        glDeleteProgram(_shader_program);
    }

    void bind() {
        glUseProgram(_shader_program);
    }
    
    void set_color(const glm::vec4& color) {
        glUniform4f(16, color.r, color.g, color.b, color.a);
        glUniform1i(17, 1);  // enable uniform color
    }
    
    void use_uniform_color(bool enabled) {
        glUniform1i(17, enabled ? 1 : 0);
    }

    void use_texture(bool enabled) {
        glUniform1i(18, enabled ? 1 : 0);
    }

    void set_emissive(bool enabled) {
        glUniform1i(19, enabled ? 1 : 0);
    }

    void set_light(int index, const glm::vec3& pos, const glm::vec3& col) {
        if (index < 0 || index >= 4) return;
        glUniform3f(20 + index, pos.x, pos.y, pos.z);
        glUniform3f(30 + index, col.r, col.g, col.b);
    }

    void use_lighting(bool enabled) {
        glUniform1i(40, enabled ? 1 : 0);
    }

    void clear_color_override() {
        use_uniform_color(false);
        set_emissive(false);
    }

    GLuint _shader_program;
};