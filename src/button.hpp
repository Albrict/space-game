#pragma once
#include "label.hpp"

class Button : public Widget {
    SDL_Color button_color;
    SDL_Color text_color;
    SDL_Texture *button_texture;
     
    Label *label;
public:
    // Constructor for simple button without texture
    Button(SDL_Renderer &render, const SDL_Rect p_button_rect, const SDL_Color color = {0, 0, 0, 0}, const std::string &p_label = "", 
           const SDL_Color p_text_color = {0, 0, 0, 0}, TTF_Font *font = nullptr);
    // Constuctor for button with custom texture
    Button(SDL_Renderer &render, const SDL_Rect p_button_rect, SDL_Texture *texture, const std::string &p_label = "",
           const SDL_Color text_color = {0, 0, 0, 0}, TTF_Font *font = nullptr);
    virtual ~Button()
    { SDL_DestroyTexture(button_texture); delete label; }

    void draw(SDL_Renderer &render) noexcept;
    void proccessEvents(SDL_Event &e) const noexcept;
private:
    bool isMouseOnWidget(const SDL_Point mouse) const noexcept;
};
