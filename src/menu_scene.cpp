#include <SDL_log.h>
#include <SDL_image.h>

#include "button.hpp"
#include "globals.h"
#include "menu_scene.hpp"
#include "game.hpp"
#include "colors.h"

namespace {
    const std::string title_text = "Space!"; 
    const std::string play_text = "Play";
    const std::string quit_text = "Quit";
    const std::string font_path = "../assets/ARCADECLASSIC.TTF"; 

    constexpr auto font_size = 28;

    const char *scene_log = "Loaded menu scene!\n";
};

MenuScene::MenuScene(SDL_Renderer &render)
{
    // Init widgets
    int text_title_size = title_text.size() * globals::font_size;
    SDL_Rect title_rect = {globals::window_width / 2 - text_title_size, 0, globals::window_width / 2, 120 };
    SDL_Rect button_rect = {static_cast<int>(globals::window_width / 2 - play_text.size() * globals::font_size), 
        globals::window_height / 6, globals::button_width + globals::font_size, globals::button_height};
    
    font = TTF_OpenFont(font_path.c_str(), font_size);

    auto &title = *new Label(render, title_rect, colors::text_color, title_text, *font);

    auto &play = *new Button(render, button_rect, colors::none, play_text, colors::text_color, font);
    play.setCallBackFunction(play_callback, this);

    button_rect.y = globals::window_height / 3;
    auto &quit = *new Button(render, button_rect, colors::none, quit_text, colors::text_color, font);
    quit.setCallBackFunction(quit_callback, this);
    
    widget_array[0] = &title;
    widget_array[1] = &play;
    widget_array[2] = &quit;

    // Init background
    background = new Texture(render, "../assets/menu_background.png");
    SDL_Log("%s", scene_log);
}

MenuScene::~MenuScene()
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

void MenuScene::proccessEvents(SDL_Event &e) noexcept
{
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            subject.notify(event::EXIT);
        for (auto const &widget : widget_array)
            widget->proccessEvents(e);
    }
}

void MenuScene::update() noexcept
{
    --scrolling_offset;
    if (scrolling_offset < -background->getTextureRect().w)
        scrolling_offset = 0;
}

void MenuScene::draw(SDL_Renderer &render) noexcept
{
    renderScrollingBackground(render); 
    for (auto const &widget: widget_array)
        widget->draw(render);
}

void MenuScene::quit_callback(void *data)
{
    MenuScene &menu_scene = *(MenuScene*)data;
    menu_scene.subject.notify(event::EXIT);
}

void MenuScene::play_callback(void *data)
{
    MenuScene &menu_scene = *(MenuScene*)data;
    menu_scene.subject.notify(event::PLAY);
}

void MenuScene::renderScrollingBackground(SDL_Renderer &render)
{
    SDL_Rect render_rect = {scrolling_offset, 0, background->getTextureRect().w, background->getTextureRect().h};
    SDL_RenderCopy(&render, &background->getTexture(), nullptr, &render_rect);
    render_rect.x = scrolling_offset + background->getTextureRect().w;
    SDL_RenderCopy(&render, &background->getTexture(), nullptr, &render_rect);
}
