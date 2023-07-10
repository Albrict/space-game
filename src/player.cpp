#include "globals.h"
#include "player.hpp"
#include "projectile.hpp"

namespace {
    const Vector2D initial_direction = {0.f, 0.f };
    constexpr auto velocity = 40.f;  

    const std::filesystem::path projectile_asset_path = "../assets/projectile.png";
};

Player::Player(SDL_Renderer &render, const SDL_FRect player_rect, 
               Texture &ship_texture, Texture &p_projectile_texture, std::list<Object*> &p_object_list)
  : Object(ship_texture, player_rect, object_type::PLAYER, velocity, 270.0), 
    object_list(p_object_list), projectile_texture(p_projectile_texture)
{
    ;
}

void Player::proccessEvents(SDL_Event &e) noexcept
{

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case 'w':
            case 'W':   increaseObjectVelocity(10);    break;

            case 's':
            case 'S':   decreaseObjectVelocity(10);   break;

            case 'a':
            case 'A':   decreaseAngle(10);  break;

            case 'd':   
            case 'D':   increaseAngle(10); break;

        }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {
        if (e.key.keysym.sym == ' ')
            fire(); 
    }
}

void Player::draw(SDL_Renderer &render) noexcept
{
    auto &player_texture = Object::getTexture().getTexture();
    auto texture_rect = Object::getTexture().getTextureRect();
    auto player_rect = Object::getObjectRect();
    SDL_RenderCopyExF(&render, &player_texture, &texture_rect,
                      &player_rect, Object::getAngle(), nullptr, SDL_FLIP_HORIZONTAL);
    
}

void Player::update(float time_step) noexcept
{
    auto angle = Object::getAngle();

    if (angle > 360)
        setAngle(0);
    else if (angle < 0)
         setAngle(360);
    
    angle = Object::getAngle();
    direction.setX(SDL_cosf(Object::degreeToRadians(angle)) * Object::getObjectVelocity());
    direction.setY(SDL_sinf(degreeToRadians(angle)) * Object::getObjectVelocity());

    object_rect.x += direction.getX() * time_step;
    object_rect.y += direction.getY() * time_step;
    if (object_rect.x < 0)
        object_rect.x = 0;
    else if (object_rect.x > globals::window_width - object_rect.w)
        object_rect.x = globals::window_width - object_rect.w;

    if (object_rect.y < 0)
        object_rect.y = 0;
    else if (object_rect.y > globals::window_height - object_rect.h)
        object_rect.y = globals::window_height - object_rect.h;
    
    if (Object::isCollided())
        Object::kill();
}

void Player::fire() noexcept
{
    auto texture_rect = projectile_texture.getTextureRect();
    SDL_FRect projectile_rect;

    projectile_rect.x = Object::getObjectRect().x + 10; 
    projectile_rect.y = Object::getObjectRect().y;
    projectile_rect.w = static_cast<float>(texture_rect.w) * 2;
    projectile_rect.h = static_cast<float>(texture_rect.h) * 2;
    
    auto angle = Object::getAngle();
    auto *projectile = new Projectile(projectile_rect, projectile_texture, angle);
    object_list.push_back(projectile);
}
