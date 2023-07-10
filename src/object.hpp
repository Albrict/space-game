#pragma once
#include <SDL_rect.h>
#include <string>

#include "SDL_events.h"
#include "texture.hpp"
#include "vector2d.hpp"

enum class object_type {
    PLAYER = 0,
    PROJECTILE,
    ASTEROID
};

class Object {
private:
    Texture &texture;
    double angle;
    bool alive = true;
    bool collision = false;

    float object_velocity;
    object_type type;
protected:
    SDL_FRect object_rect;
    Vector2D direction = {0, 0};
public:
    Object(Texture &object_texture, const SDL_FRect p_object_rect, const object_type p_type, 
           const float p_object_velocity = 0, const double p_angle = 0) noexcept
        : texture(object_texture), angle(p_angle), object_velocity(p_object_velocity), type(p_type), object_rect(p_object_rect) {}
    virtual ~Object() { }
    
    virtual void update(const float time_step) noexcept = 0;
    virtual void proccessEvents(SDL_Event &e) noexcept = 0;
    virtual void draw(SDL_Renderer &render) noexcept = 0;

    SDL_FRect getObjectRect() const noexcept
    { return object_rect; }
    
    const bool isAlive() const noexcept
    { return alive; }

    const bool isCollided() const noexcept
    { return collision; }
    void collide() noexcept
    { collision = true; }
    void uncollide() noexcept
    { collision = false; }

    object_type getObjectType() const noexcept
    { return type; }
protected:
    constexpr static double const PI = 3.14;

    Texture &getTexture() const noexcept
    { return texture; }

    constexpr double degreeToRadians(const double degree) const noexcept
    { return degree * PI / 180; }
    constexpr double radiansToDegree(const double radians) const noexcept
    { return radians * (PI / 180); }

    float getObjectVelocity() const noexcept
    { return object_velocity; }
    void increaseObjectVelocity(const float velocity) noexcept
    { object_velocity += velocity; }
    void decreaseObjectVelocity(const float velocity) noexcept
    { object_velocity -= velocity; }

    double getAngle() const noexcept
    { return angle; }
    void setAngle(const float new_angle) noexcept
    { angle = new_angle; }
    void increaseAngle(const float p_angle) noexcept
    { angle += p_angle; }
    void decreaseAngle(const float p_angle) noexcept
    { angle -= p_angle; }

    void kill() noexcept
    { alive = false; }
};
