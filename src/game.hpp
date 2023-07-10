#include "scene.hpp"
#include "observer.hpp"

class Game{
    bool running;
    SDL_Renderer *render = nullptr;
    SDL_Window *window = nullptr;
    Scene *current_scene = nullptr;

    Observer observer;
public:
    Game();
    ~Game();
    void run() noexcept;
private:
    void proccessScene(SDL_Event *e);

    void startGame();
    void endGame();
    void restartGame();
};
