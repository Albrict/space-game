#pragma once

enum class event {
    NONE,
    EXIT,
    PLAY,
    GAME_OVER,
    RESTART
};

class Observer {
    enum event event = event::NONE;
public:
    Observer() {}
    void update(const enum event p_event) noexcept
    { event = p_event; }
    enum event getEvent() const noexcept
    { return event; }
};
