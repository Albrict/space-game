#pragma once
#include "object.hpp"

class Asteroid : public Object {
    int health = 100;
public:
    Asteroid(const SDL_FRect asteroid_rect, Texture &texture, const float p_object_velocity, const float angle)
        : Object(texture, asteroid_rect, object_type::ASTEROID, p_object_velocity, angle) {}

    ~Asteroid() {}

    void proccessEvents(SDL_Event &e) noexcept;
    void draw(SDL_Renderer &render) noexcept;
    void update(const float time_step) noexcept;
};
