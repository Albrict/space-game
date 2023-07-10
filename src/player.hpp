#pragma once
#include <list>

#include "object.hpp"

class Player : public Object {
    std::list<Object*> &object_list;
    Texture &projectile_texture;
public:

    Player(SDL_Renderer &render, const SDL_FRect player_rect, Texture &ship_texture, Texture &p_projectile_texture,
           std::list<Object*> &p_object_list);
    void proccessEvents(SDL_Event &e) noexcept;
    void draw(SDL_Renderer &render) noexcept;
    void update(const float time_step) noexcept;
private:
    Player(const Player &) = delete;
    Player(Player &&) = delete;
    Player &operator=(const Player &) = delete;
    Player &operator=(Player &&) = delete;

    void fire() noexcept;
};
