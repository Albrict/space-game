#pragma once
#include <SDL_ttf.h>

#include "widget.hpp"

class Label : public Widget {
    SDL_Color text_color;
    SDL_Texture *text_texture;
    
    std::string text_label;
    TTF_Font &label_font;
public:
    Label(SDL_Renderer &render, const SDL_Rect p_text_rect, const SDL_Color color, const std::string &p_text_label, TTF_Font &font);
    ~Label()
    { SDL_DestroyTexture(text_texture); text_texture = nullptr; }

    void draw(SDL_Renderer &render) noexcept;
    void proccessEvents(SDL_Event &e) const noexcept;
    void updateText(SDL_Renderer &render, const std::string &new_value);
};
