#pragma once
#include "object.hpp"
#include "vector2d.hpp"

#include <vector>

enum class statusOfObject {
    ALIVE,
    DESTROYED
};

enum class collision {
    NONE,
    DETECTED
};


class DynamicObject : public Object {
private:
    int health = 100;

    statusOfObject status = statusOfObject::ALIVE;
    object_type type;
protected:
public:
    DynamicObject(const SDL_FRect object_rect, Texture &texture, const float p_object_velocity, 
                  const object_type p_type, const double p_angle) 
        : Object(object_rect, texture), angle(p_angle), object_velocity(p_object_velocity), type(p_type) {}

    virtual void proccessEvents(SDL_Event &e) noexcept = 0;
    virtual void draw(SDL_Renderer &render) noexcept = 0;
    virtual void update(const float time_step) noexcept = 0;
    
    statusOfObject getStatus() const noexcept
    { return status; }
    void setStatus(const statusOfObject p_status) noexcept
    { status = p_status; }
    
    int getHealth() const noexcept
    { return health; }
    void damageObject(const int damage) noexcept
    { health -= damage ; }

    
protected:
};
