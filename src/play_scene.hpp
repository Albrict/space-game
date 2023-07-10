#pragma once 
#include <list>
#include <array>
#include <SDL_ttf.h>

#include "label.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "timer.h"
#include "sprite_sheet.hpp"

class PlayScene: public Scene {
    SDL_Renderer &render;
    LTimer timer;
    std::list<Object*> object_list;
    std::array<Label*, 2> label_array; 
    std::array<Texture*, 3> texture_array;

    SDL_TimerID timer_callback_id;
    SpriteSheet *sprite_sheet = nullptr;
    TTF_Font *font = nullptr;

    int score = 0;
public:
    PlayScene(SDL_Renderer &render);
    ~PlayScene();
    
    void proccessEvents(SDL_Event &e) noexcept;
    void update() noexcept;
    void draw(SDL_Renderer &render) noexcept;

private:
    void deleteDestroyedObjects() noexcept;
    void proccessCollision() noexcept;
    void checkCollision(Object &object);

    static Uint32 spawn_asteroids_callback(Uint32 interval, void *data);
};
