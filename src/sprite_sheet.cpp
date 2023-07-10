#include "sprite_sheet.hpp"

#include <SDL2/SDL_image.h>
#include <stdexcept>

namespace {
    const std::string faildet_to_load("Failed to load: ");
    const std::string sprite_sheet_cols_warning("Sprite sheet is not loaded properly. Width of sprites not even\n");
    const std::string sprite_sheet_rows_warning("Sprite sheet is not loaded properly. Height of sprites not even\n");
}


SpriteSheet::SpriteSheet(const std::filesystem::path &path_to_texture, 
            const Uint16 p_sprite_width, const Uint16 p_sprite_height, SDL_Renderer &render)
        : texture_vector(), sprite_width(p_sprite_width), sprite_height(p_sprite_height)
{
    auto sprite_sheet_clips = 0;
    auto sprite_sheet_rows = 0;

    // Load sprite_sheet texture
    SDL_Texture *sprite_sheet_texture = IMG_LoadTexture(&render, path_to_texture.c_str());
    if (sprite_sheet_texture == nullptr)
        throw std::filesystem::filesystem_error(IMG_GetError(), path_to_texture, std::error_code());
    
    // Initializing sprite_sheet texture
    sprite_sheet = new Texture(*sprite_sheet_texture);
    sprite_sheet_clips = sprite_sheet->getTextureRect().w / sprite_width;
    sprite_sheet_rows = sprite_sheet->getTextureRect().h / sprite_height;

    // Add fallback
    if ((sprite_sheet_clips % 2) != 0)
        SDL_Log("%s", sprite_sheet_cols_warning.c_str());

    if (((sprite_sheet_rows != 1)) && ((sprite_sheet_rows % 2) != 0))
        SDL_Log("%s", sprite_sheet_rows_warning.c_str());
    
    loadSprites(render, sprite_sheet_clips, sprite_sheet_rows);
}

SpriteSheet::~SpriteSheet()
{
    for (const auto &sprite : texture_vector)
        delete sprite;
    texture_vector.clear();

    delete sprite_sheet;
}

void SpriteSheet::loadSprites(SDL_Renderer &render, const int clips, const int rows) noexcept
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < clips; ++j) {
            SDL_Rect sprite_clip = {j * sprite_width, i * sprite_height, sprite_width, sprite_height};
            auto &texture = getPartOfTexture(render, sprite_clip, *sprite_sheet);
            texture_vector.push_back(&texture);
        }
    }
}

Texture &SpriteSheet::getPartOfTexture(SDL_Renderer &render, SDL_Rect texture_rect, Texture &source) const noexcept
{
    SDL_Texture *texture = SDL_CreateTexture(&render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
                                             sprite_width, sprite_height);
    SDL_SetRenderTarget(&render, texture);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(&render);
    SDL_RenderCopy(&render, &source.getTexture(), &texture_rect, nullptr);
    SDL_SetRenderTarget(&render, nullptr);
     
    texture_rect.x = 0;
    texture_rect.y = 0;

    return *new Texture(*texture);
}
