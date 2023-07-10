#include "game_over_scene.hpp"
#include "globals.h"
#include "button.hpp"
#include "colors.h"

namespace {
    const std::string game_over_text = "Game Over!";
    const std::string play_again_text = "Restart!";
    const std::string quit_text = "Exit";

    const std::string font_path = "../assets/ARCADECLASSIC.TTF"; 

    constexpr auto font_size = 28;
};

GameOverScene::GameOverScene(SDL_Renderer &render)
{
    int text_title_size = game_over_text.size() * globals::font_size;
    SDL_Rect game_over_rect = {globals::window_width / 2 - text_title_size, 0, globals::window_width / 2, 120 };
    SDL_Rect button_rect = {static_cast<int>(globals::window_width / 2 - play_again_text.size() * font_size), 
        globals::window_height / 6, globals::button_width + globals::font_size, globals::button_height};

    font = TTF_OpenFont(font_path.c_str(), font_size);
    
    auto &game_over_label = *new Label(render, game_over_rect, colors::text_color, game_over_text, *font);
    
    auto &play_again = *new Button(render, button_rect, colors::none, play_again_text, colors::text_color, font);
    play_again.setCallBackFunction(restartCallback, this);
    
    button_rect.y = globals::window_height / 3;
    auto &quit = *new Button(render, button_rect, colors::none, quit_text, colors::text_color, font);
    quit.setCallBackFunction(quitCallback, this);

    widget_array[0] = &game_over_label;
    widget_array[1] = &play_again;
    widget_array[2] = &quit;

    background = new Texture(render, "../assets/menu_background.png");
}

GameOverScene::~GameOverScene()
{
    for (auto &widget : widget_array) {
        delete widget;
        widget = nullptr;
    }
    delete background;
    background = nullptr;

    TTF_CloseFont(font);
    font = nullptr;
}

void GameOverScene::proccessEvents(SDL_Event &e) noexcept
{
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            subject.notify(event::EXIT);
        for (auto const &widget : widget_array)
            widget->proccessEvents(e);
    }
}

void GameOverScene::update() noexcept
{
    --scrolling_offset;
    if (scrolling_offset < -background->getTextureRect().w)
        scrolling_offset = 0;
}

void GameOverScene::draw(SDL_Renderer &render) noexcept
{
    renderScrollingBackground(render);
    for (auto const &widget: widget_array)
        widget->draw(render);
}

void GameOverScene::renderScrollingBackground(SDL_Renderer &render)
{
    SDL_Rect render_rect = {scrolling_offset, 0, background->getTextureRect().w, background->getTextureRect().h};
    SDL_RenderCopy(&render, &background->getTexture(), nullptr, &render_rect);
    render_rect.x = scrolling_offset + background->getTextureRect().w;
    SDL_RenderCopy(&render, &background->getTexture(), nullptr, &render_rect);
}

void GameOverScene::quitCallback(void *data)
{
    GameOverScene &game_over_scene = *(GameOverScene*)data;
    game_over_scene.subject.notify(event::EXIT);
}

void GameOverScene::restartCallback(void *data)
{
    GameOverScene &game_over_scene = *(GameOverScene*)data;
    game_over_scene.subject.notify(event::RESTART);
}
