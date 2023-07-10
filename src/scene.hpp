#pragma once
#include <SDL_events.h>
#include <SDL_render.h>
#include "subject.hpp"


class Scene {
protected:
    Subject subject;
public:
    Scene() 
        : subject() {}
    virtual ~Scene() {}
    
    virtual void proccessEvents(SDL_Event &e) noexcept = 0;
    virtual void update() noexcept = 0;
    virtual void draw(SDL_Renderer &render) noexcept = 0;

    void attachObserver(Observer &observer)
    { subject.attach(observer); }
    void detachObserver(Observer &observer)
    { subject.detach(observer); }
};
