#pragma once

struct Texture {
    void init(const std::string& path) {
        // base directory of the executable
        std::string base_path = SDL_GetBasePath();
        std::string full_path = base_path + "assets/textures/" + path;
        
        // load image from disk
        SDL_Surface* texture_raw_p = IMG_Load(full_path.c_str());
        if (texture_raw_p == nullptr) {
            std::println("IMG_Load(\"{}\"): {}", path, SDL_GetError());
            std::exit(1);
        }


        // store width and height
        _width = texture_raw_p->w;
        _height = texture_raw_p->h;

        const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(texture_raw_p->format);
        bool has_alpha = details->bits_per_pixel == 32;
        GLenum internal_format = has_alpha ? GL_SRGB8_ALPHA8 : GL_SRGB8;
        GLenum data_format = has_alpha ? GL_RGBA : GL_RGB;

        // create texture to store image in gpu buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &_texture);

        // upload to the GPU
        glTextureStorage2D(_texture, 1, internal_format, _width, _height);
        glTextureSubImage2D(_texture, 0, 0, 0, _width, _height, data_format, GL_UNSIGNED_BYTE, texture_raw_p->pixels);

        // free image on cpu side
        SDL_DestroySurface(texture_raw_p);

        glGenerateTextureMipmap(_texture);
        glTextureParameteri(_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // interpolation mode when scaling image down
        glTextureParameteri(_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);               // interpolation mode when scaling image up
    }
    void destroy() {
        glDeleteTextures(1, &_texture);
    }
    void bind() {
        glBindTextureUnit(0, _texture);
    }

    GLuint _texture;
    GLuint _width;
    GLuint _height;
};