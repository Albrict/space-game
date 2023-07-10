#pragma once
#include <vector>
#include <string>

#include "texture.hpp"


class SpriteSheet {
    std::vector<Texture*> texture_vector;
    Texture *sprite_sheet = nullptr;

    int sprite_width = 0;
    int sprite_height = 0;
public:
    SpriteSheet(const std::filesystem::path &path_to_texture, 
                const Uint16 p_sprite_width, const Uint16 p_sprite_height, SDL_Renderer &render);
    ~SpriteSheet();
    
    Texture &getSprite(const size_t sprite_id) const noexcept
    { return *texture_vector.at(sprite_id); }
    Texture &getSpriteSheetTexture() const noexcept
    { return *sprite_sheet; }
private:
    void loadSprites(SDL_Renderer &render, const int clips, const int rows) noexcept;
    Texture &getPartOfTexture(SDL_Renderer &render, SDL_Rect texture_rect, Texture &source) const noexcept;
};
