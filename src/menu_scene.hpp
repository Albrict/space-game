#pragma once
#include <SDL_ttf.h>
#include <array>

#include "scene.hpp"
#include "texture.hpp"
#include "widget.hpp"

class MenuScene : public Scene {
    std::array<Widget*, 3> widget_array;
    Texture *background = nullptr;
    int scrolling_offset = 0;

    TTF_Font *font = nullptr;
public:
    MenuScene(SDL_Renderer &render);
    ~MenuScene();
    
    void proccessEvents(SDL_Event &e) noexcept;
    void update() noexcept;
    void draw(SDL_Renderer &render) noexcept;
private:
    static void quit_callback(void *data);
    static void play_callback(void *data);

    void renderScrollingBackground(SDL_Renderer &render);
};
