#pragma once

#include <iostream>
#include <memory>
#include "entity.h"

class render_component : public component
{
private:
    entity _parent;
public:
    render_component(entity parent)
    : _parent(parent)
    {
    }

    bool initialise()
    { 
        std::cout << "Render component initialised" << std::endl; 
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "Render component update" << std::endl;
    }

    void render()
    {
        std::cout << "Render component render" << std::endl;
    }

    void shutdown()
    {
        std::cout << "Render component shutdown" << std::endl;
    }
};