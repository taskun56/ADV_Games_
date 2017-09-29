#pragma once

#include <iostream>
#include <memory>
#include "entity.h"

class physics_component : public component
{
private:
    entity _parent;
public:
    physics_component(entity parent)
    : _parent(parent)
    {
    }

    bool initialise()
    { 
        std::cout << "Physics component initialised" << std::endl; 
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "Physics component update" << std::endl;
    }

    void render()
    {
        std::cout << "Physics component render" << std::endl;
    }

    void shutdown()
    {
        std::cout << "Physics component shutdown" << std::endl;
    }
};