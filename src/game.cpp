#include "esh_error.h"
#include "game.hpp"
#include "globals.h"
#include "menu_scene.hpp"
#include "play_scene.hpp"
#include "game_over_scene.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

Game::Game()
    : running(true), observer()
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
    current_scene->attachObserver(observer);
}

Game::~Game() 
{
    delete current_scene;
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::run() noexcept
{
    SDL_Event e;
    while(running) {
        SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(render);
        proccessScene(&e);
        SDL_RenderPresent(render);
    }
}

void Game::proccessScene(SDL_Event *e)
{
    switch(observer.getEvent()) {
    case event::NONE:
        break;
    case event::EXIT:
        running = false;
        observer.update(event::NONE);
        break;
    case event::PLAY:
        startGame();
        observer.update(event::NONE);
        break;
    case event::GAME_OVER:
        endGame();
        observer.update(event::NONE);
        break;
    case event::RESTART:
        restartGame();
        observer.update(event::NONE);
        break;
    }
    current_scene->proccessEvents(*e);
    current_scene->update();
    current_scene->draw(*render);
}

void Game::startGame()
{
    delete current_scene;
    current_scene = new PlayScene(*render);
    current_scene->attachObserver(observer);
}

void Game::endGame()
{
    delete current_scene; 
    current_scene = new GameOverScene(*render);
    current_scene->attachObserver(observer);
}

void Game::restartGame()
{
    delete current_scene; 
    current_scene = new PlayScene(*render);
    current_scene->attachObserver(observer);
}
