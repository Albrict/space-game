#include "button.hpp"

Button::Button(SDL_Renderer &render, const SDL_Rect p_button_rect, const SDL_Color color, const std::string &p_label,
              const SDL_Color p_text_color, TTF_Font *font)
    : Widget(p_button_rect), button_color(color), text_color(p_text_color), button_texture(nullptr)
{
    if (p_label.size() == 0) {
        label = nullptr;
    } else {
        if (font) {
            label = new Label(render, p_button_rect, text_color, p_label, *font);
        }
    }
}

Button::Button(SDL_Renderer &render, const SDL_Rect p_button_rect, SDL_Texture *texture, const std::string &p_label,
              const SDL_Color text_color, TTF_Font *font)
    : Widget(p_button_rect), text_color(text_color), button_texture(texture)
{
    if (p_label.size() == 0) {
        label = nullptr;
    } else {
        if (font) {
            label = new Label(render, p_button_rect, text_color, p_label, *font);
        }
    }
}

void Button::draw(SDL_Renderer &render) noexcept
{
    if (button_texture) {
        SDL_RenderCopy(&render, button_texture, nullptr, &widget_rect); 
        if (label)
            label->draw(render);
    } else {
//        SDL_SetRenderDrawColor(&render, button_color.r, button_color.g, button_color.b, button_color.a);
//        SDL_RenderFillRect(&render, &widget_rect);
        if (label)
            label->draw(render);
    }
}

void Button::proccessEvents(SDL_Event &e) const noexcept
{
    SDL_Point mouse;
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if (SDL_PointInRect(&mouse, &widget_rect))
            callback_function(data);
    }
}

bool Button::isMouseOnWidget(const SDL_Point mouse) const noexcept
{
    if (mouse.x < widget_rect.x)
        return false;
    else if (mouse.x > widget_rect.x + widget_rect.w)
        return false;
    else if (mouse.y < widget_rect.y)
        return false;
    else if (mouse.y > widget_rect.y + widget_rect.h)
        return false;
    else  
        return true;
}
