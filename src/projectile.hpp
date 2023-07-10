#include "object.hpp"

class Projectile : public Object {
    const int projectile_damage = 20;
public:
    Projectile(const SDL_FRect projectile_rect, Texture &texture, const double angle)
        : Object(texture, projectile_rect, object_type::PROJECTILE, 1000.f, angle) {}
    ~Projectile() {}
    
    void proccessEvents(SDL_Event &e) noexcept;
    void draw(SDL_Renderer &render) noexcept;
    void update(const float time_step) noexcept;
};
