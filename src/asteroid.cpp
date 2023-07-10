#include "asteroid.hpp"

void Asteroid::proccessEvents(SDL_Event &e) noexcept
{
     
}

void Asteroid::draw(SDL_Renderer &render) noexcept
{
    auto &asteroid_texture = Object::getTexture();
    auto texture_rect = asteroid_texture.getTextureRect();
    auto asteroid_rect = Object::getObjectRect(); SDL_RenderCopyExF(&render, &asteroid_texture.getTexture(), &texture_rect,
                      &asteroid_rect, Object::getAngle(), nullptr, SDL_FLIP_NONE);
}

void Asteroid::update(const float time_step) noexcept
{
    auto angle = Object::getAngle();

    if (angle > 360)
        setAngle(0);
    else if (angle < 0)
         setAngle(360);
    
    angle = Object::getAngle();
    direction.setX(SDL_cosf(degreeToRadians(angle)) * Object::getObjectVelocity());
    direction.setY(SDL_sinf(degreeToRadians(angle)) * Object::getObjectVelocity());

    object_rect.x += direction.getX() * time_step;
    object_rect.y += direction.getY() * time_step;

    if (Object::isCollided()) {
        health -= 20;
        Object::uncollide();
    }
    if (health < 0)
        Object::kill();
}
