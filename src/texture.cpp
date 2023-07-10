#include "texture.hpp"
#include <SDL_image.h>
#include <filesystem>

namespace {
    constexpr auto error_null_argument = "p_texture is nullptr!";
};

Texture::Texture(SDL_Renderer &render, const std::filesystem::path &path)
{
    auto loaded_texture = IMG_LoadTexture(&render, path.c_str());
    if (loaded_texture == nullptr)
        throw std::filesystem::filesystem_error(IMG_GetError(), path, std::error_code());
    else
        texture = loaded_texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &texture_rect.w, &texture_rect.h);
}

Texture::Texture(SDL_Texture &p_texture)
{
    texture = &p_texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &texture_rect.w, &texture_rect.h);
}
