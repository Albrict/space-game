#pragma once
#include "SDL_events.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <string>
#include <functional>

class Widget {
protected:
    SDL_Rect widget_rect;
    std::function<void(void*)> callback_function;
    void *data = nullptr;
public:
    Widget(const SDL_Rect p_widget_rect)
        : widget_rect(p_widget_rect), callback_function(nullptr) {}
    virtual ~Widget() {};

    virtual void draw(SDL_Renderer &render) noexcept = 0;
    virtual void proccessEvents(SDL_Event &e) const noexcept = 0;
    virtual void move(const int x, const int y)
    { widget_rect.x = x; widget_rect.y = y; }

    virtual void setCallBackFunction(std::function<void(void*)> p_callback_function, void *p_data) noexcept
    { callback_function = p_callback_function; data = p_data; }
};

