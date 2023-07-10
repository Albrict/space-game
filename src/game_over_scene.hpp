#pragma once
#include <array>
#include <SDL_ttf.h>

#include "scene.hpp"
#include "texture.hpp"
#include "widget.hpp"

class GameOverScene: public Scene {
    std::array<Widget*, 3> widget_array;
    Texture *background = nullptr;
    int scrolling_offset = 0;

    TTF_Font *font = nullptr;
public:
    GameOverScene(SDL_Renderer &render);
    ~GameOverScene();
    
    void proccessEvents(SDL_Event &e) noexcept;
    void update() noexcept;
    void draw(SDL_Renderer &render) noexcept;
private:
    void renderScrollingBackground(SDL_Renderer &render); 
    static void quitCallback(void *data);
    static void restartCallback(void *data);
};
