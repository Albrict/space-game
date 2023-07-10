#pragma once
#include <SDL_render.h>
#include <filesystem>

class Texture {
    SDL_Texture *texture = nullptr;
    SDL_Rect texture_rect = {0, 0, 0, 0};
public:
    Texture(SDL_Renderer &render, const std::filesystem::path &path);
    Texture(SDL_Texture &p_texture);
    ~Texture()
    { SDL_DestroyTexture(texture); }
    SDL_Rect getTextureRect() noexcept
    { return texture_rect; }
    SDL_Texture &getTexture() const noexcept
    { return *texture; }
};
