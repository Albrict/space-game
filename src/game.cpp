#include "esh_error.h"
#include "game.hpp"
#include "globals.h"
#include "menu_scene.hpp"
#include "play_scene.hpp"
#include "game_over_scene.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace ShSpace {
    SDL_Window *window = nullptr;
    SDL_Renderer *render = nullptr;
    Scene *current_scene = nullptr;
    std::function<void(SDL_Renderer &, SDL_Event &e)> current_state;
    auto running = false;

    Observer game_observer;
    
    void proccessScene(SDL_Event &e);
    void startGame();
    void endGame();
    void restartGame();

    void init() noexcept
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            die(SDL_GetError());
        
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
            die(IMG_GetError());

        if (TTF_Init() == -1)
            die(TTF_GetError());

        window = SDL_CreateWindow("ShSpace", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, globals::window_width, 
                                  globals::window_height, SDL_WINDOW_SHOWN);
        if (window == nullptr)
            die(SDL_GetError());

        render = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        if (render == nullptr)
            die(SDL_GetError());
        
        current_scene = new MenuScene(*render);
        current_scene->attachObserver(game_observer);
        running = true;
    }

    void run() 
    {   
        SDL_Event e;
        while(running) {
            SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(render);
            proccessScene(e);
            SDL_RenderPresent(render);
        }
    }

    void quit() noexcept
    {
        delete current_scene;
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    void proccessScene(SDL_Event &e)
    {
        switch(game_observer.getEvent()) {
        case event::NONE:
            break;
        case event::EXIT:
            running = false;
            game_observer.update(event::NONE);
            break;
        case event::PLAY:
            startGame();
            game_observer.update(event::NONE);
            break;
        case event::GAME_OVER:
            endGame();
            game_observer.update(event::NONE);
            break;
        case event::RESTART:
            restartGame();
            game_observer.update(event::NONE);
            break;
        }
        current_scene->proccessEvents(e);
        current_scene->update();
        current_scene->draw(*render);
    }

    void startGame()
    {
        delete current_scene;
        current_scene = new PlayScene(*render);
        current_scene->attachObserver(game_observer);
    }

    void endGame()
    {
        delete current_scene; 
        current_scene = new GameOverScene(*render);
        current_scene->attachObserver(game_observer);
    }

    void restartGame()
    {
        delete current_scene; 
        current_scene = new PlayScene(*render);
        current_scene->attachObserver(game_observer);
    }
}
