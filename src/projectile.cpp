#include "projectile.hpp"
#include <SDL_rect.h>

void Projectile::proccessEvents(SDL_Event &e) noexcept
{
    ;
}

void Projectile::draw(SDL_Renderer &render) noexcept
{
    auto &projectile_texture = Object::getTexture();
    auto texture_rect = projectile_texture.getTextureRect();
    auto projectile_rect = Object::getObjectRect();
    
    SDL_RenderCopyExF(&render, &projectile_texture.getTexture(), &texture_rect,
                      &projectile_rect, Object::getAngle(), nullptr, SDL_FLIP_VERTICAL);
}

void Projectile::update(const float time_step) noexcept
{
    auto angle = Object::getAngle();

    if (angle > 360)
        setAngle(0);
    else if (angle < 0)
         setAngle(360);
    
    angle = Object::getAngle();

    direction.setX(SDL_cosf(Object::degreeToRadians(angle)) * Object::getObjectVelocity());
    direction.setY(SDL_sinf(Object::degreeToRadians(angle)) * Object::getObjectVelocity());

    object_rect.x += direction.getX() * time_step;
    object_rect.y += direction.getY() * time_step;

    if (Object::isCollided())
        Object::kill();
}
