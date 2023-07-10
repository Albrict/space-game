#pragma once
#include "observer.hpp"
#include <list>

class Subject {
    std::list<Observer*> observer_list;
public:
    Subject()
        : observer_list() {}
    void attach(Observer& observer)
    { observer_list.push_back(&observer); }
    void detach(Observer &observer)
    { observer_list.remove(&observer); }

    void notify(const enum event event)
    {
        for (const auto &observer : observer_list)
            observer->update(event);
    }
};
