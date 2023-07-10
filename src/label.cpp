#include "label.hpp"
#include "esh_error.h"
#include <stdexcept>

Label::Label(SDL_Renderer &render, const SDL_Rect p_text_rect, const SDL_Color color, const std::string &p_text_label, TTF_Font &font)
    : Widget(p_text_rect), text_label(p_text_label), label_font(font)
{
    text_color = color;
    SDL_Surface *text_surface = TTF_RenderText_Solid(&label_font, text_label.c_str(), text_color);

    if (text_surface == nullptr)
        throw std::runtime_error(TTF_GetError());
    
    text_texture = SDL_CreateTextureFromSurface(&render, text_surface);
    SDL_FreeSurface(text_surface);

    if (text_texture == nullptr)
        throw std::runtime_error(SDL_GetError());
}

void Label::draw(SDL_Renderer &render) noexcept
{
    SDL_RenderCopy(&render, text_texture, nullptr, &widget_rect);
}

void Label::proccessEvents(SDL_Event &e) const noexcept
{
    ;
}

void Label::updateText(SDL_Renderer &render, const std::string &new_text)
{
    text_texture = nullptr;
    text_label = new_text;

    SDL_Surface *text_surface = TTF_RenderText_Solid(&label_font, text_label.c_str(), text_color);

    if (text_surface == nullptr) {
        panic(TTF_GetError());
        return;
    }

    text_texture = SDL_CreateTextureFromSurface(&render, text_surface);
    if (text_texture == nullptr)
        panic(TTF_GetError());
        
    SDL_FreeSurface(text_surface);
}
