#include "play_scene.hpp"
#include "SDL_log.h"
#include "globals.h"
#include "label.hpp"
#include "colors.h"
#include "player.hpp"
#include "asteroid.hpp"

#include <SDL_image.h>
#include <stdexcept>
#include <sstream>

using path = std::filesystem::path;

namespace {
    const path ship_asset_path = "../assets/ship.png";
    const path play_background_path = "../assets/play_background.png";
    const path asteroids_asset_path = "../assets/asteroids.png";
    const path projectile_asset_path = "../assets/projectile.png";

    constexpr auto font_path = "../assets/ARCADECLASSIC.TTF";

    const std::string score_text = "Score ";

    const SDL_Rect score_rect = {static_cast<int>(globals::window_width / 2 - score_text.size() * 28), 0, 200, 120};
    const SDL_Rect num_rect = {static_cast<int>(globals::window_width / 2 + score_text.size() * 28), 0, 120, 120};

    enum side {
        LEFT,
        TOP,
        RIGHT,
        BOTTOM
    };
    enum scene_textures{
        BACKGROUND = 0,
        SHIP = 1,
        PROJECTILE = 2 
    };
    enum scene_labels {
        SCORE,
        NUM
    };

    bool checkBounds(const SDL_FRect object_rect)
    {
        return object_rect.x <= -50 ||
               object_rect.x >= globals::window_width + 50 ||
               object_rect.y <= -50 ||
               object_rect.y >= globals::window_height + 50;
    }
}


PlayScene::PlayScene(SDL_Renderer &render)
    : Scene(), render(render), timer(), object_list(), label_array()
{
    auto player_x = static_cast<float>(globals::window_width) / 2;
    auto player_y = static_cast<float>(globals::window_height) / 2;
    
    // Load assets
    SDL_FRect player_rect = {player_x, player_y, 120, 120 };

    try {
        auto ship_texture = new Texture(render, ship_asset_path);
        auto projectile_texture = new Texture(render, projectile_asset_path);
        auto background = new Texture(render, play_background_path);
        
        texture_array[BACKGROUND] = background;
        texture_array[SHIP] = ship_texture;
        texture_array[PROJECTILE] = projectile_texture;

        sprite_sheet = new SpriteSheet(asteroids_asset_path, 32, 32, render);
    } catch (std::exception &ex) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", ex.what()); 
        exit(EXIT_FAILURE);
    }
    font = TTF_OpenFont(font_path, 28);

    auto player = new Player(render, player_rect, *texture_array[SHIP], *texture_array[PROJECTILE], object_list);
    auto score_label = new Label(render, score_rect, colors::text_color, score_text, *font);
    auto score_num_label = new Label(render, num_rect, colors::text_color, "0", *font);
    
    object_list.push_back(player);
    label_array[SCORE] = score_label;
    label_array[NUM] = score_num_label;
    timer_callback_id = SDL_AddTimer(1 * 1000, spawn_asteroids_callback, static_cast<void*>(this));
}

PlayScene::~PlayScene()
{
    for (auto &object : object_list) {
        delete object;
        object = nullptr;
    }
    for (auto &label : label_array) {
        delete label;
        label = nullptr;
    }
    for (auto &texture : texture_array) {
        delete texture;
        texture = nullptr;
    }
    delete sprite_sheet;
    sprite_sheet = nullptr;
    
    SDL_RemoveTimer(timer_callback_id);
    TTF_CloseFont(font);
    font = nullptr;
}

void PlayScene::proccessEvents(SDL_Event &e) noexcept
{
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) 
            subject.notify(event::EXIT);
        for (const auto  &object : object_list)
            object->proccessEvents(e);
    }
}

void PlayScene::update() noexcept
{
    PlayScene::proccessCollision();
    PlayScene::deleteDestroyedObjects();
    
    std::stringstream num_str;
    num_str << score;
    label_array[NUM]->updateText(render, num_str.str());
    float time_step = timer.getTicks() / 1000.f;

    for (const auto  &object : object_list)
        object->update(time_step);
    timer.start();
}

void PlayScene::draw(SDL_Renderer &render) noexcept
{
    SDL_Rect screen_rect = {0, 0, globals::window_width, globals::window_height};
    auto &background_texture = texture_array[BACKGROUND]->getTexture();
    auto background_rect = texture_array[BACKGROUND]->getTextureRect();
    SDL_RenderCopy(&render, &background_texture, &background_rect, &screen_rect);

    for (const auto  &object : object_list)
        object->draw(render);
    for (const auto  &label : label_array)
        label->draw(render);
}

void PlayScene::checkCollision(Object &object)
{
    const auto object_rect = object.getObjectRect();
    for (const auto &other_object : object_list) {
        if (other_object->getObjectType() == object_type::PROJECTILE || other_object->getObjectType() == object_type::PLAYER) {
            continue;
        } else {
            const auto other_object_rect = other_object->getObjectRect();  
            if (SDL_HasIntersectionF(&object_rect, &other_object_rect) == SDL_TRUE) {
                object.collide();
                other_object->collide();
            }
        }
    }
}

void PlayScene::proccessCollision() noexcept 
{
    for (const auto &object : object_list) {
        if (object->getObjectType() == object_type::PLAYER || object->getObjectType() == object_type::PROJECTILE)
            PlayScene::checkCollision(*object); 
    }
}

void PlayScene::deleteDestroyedObjects() noexcept
{
    auto it = object_list.begin();
    while (it != object_list.end()) {
        SDL_FRect object_rect = (*it)->getObjectRect();
        if (!(*it)->isAlive()) {
            if ((*it)->getObjectType() == object_type::ASTEROID)
                score += 5;
            else if ((*it)->getObjectType() == object_type::PLAYER)
                subject.notify(event::GAME_OVER);
            delete (*it);
            object_list.erase(it++);
        } else if (checkBounds(object_rect)) {
            delete (*it);
            object_list.erase(it++);
        } else {
            ++it;
        }
    } 
}

Uint32 PlayScene::spawn_asteroids_callback(Uint32 interval, void *data)
{
    auto &play_scene = *(PlayScene*)data;

    int amount = rand() % 5 + 1;
    for (int i = 0; i < amount; ++i) {
        float asteroid_spawn_x = 0;
        float asteroid_spawn_y = 0; 

        float asteroid_angle = static_cast<float>(rand() / (static_cast<float>(RAND_MAX / (360))));
        float asteroid_width = rand() % 50 + 50 + 1;
        float asteroid_height = rand() % 50 + 50 + 1;

        auto sprite_id = rand() % 4;
        auto velocity = rand() % 50 + 50;
        auto side = rand() % 4;

        switch(side) {
        case LEFT:
            asteroid_spawn_y = rand() % globals::window_height;
            asteroid_spawn_x -= 30;
            break;
        case RIGHT:
            asteroid_spawn_y = rand() % globals::window_height;
            asteroid_spawn_x = globals::window_width + 30;
            break;
        case TOP:
            asteroid_spawn_x = rand() % globals::window_width;
            asteroid_spawn_y -= 30;
            break;
        case BOTTOM:
            asteroid_spawn_x = rand() % globals::window_width;
            asteroid_spawn_y = globals::window_height + 30;
            break;
        }
        
        auto &asteroid_texture = play_scene.sprite_sheet->getSprite(sprite_id);
        SDL_FRect asteroid_rect = {asteroid_spawn_x, asteroid_spawn_y, asteroid_width, asteroid_height };
        auto asteroid = new Asteroid(asteroid_rect, asteroid_texture, velocity, asteroid_angle);
        play_scene.object_list.push_back(asteroid);
    }
    return 1000;
}
