#pragma once

#include <iostream>
#include <memory>
#include "entity.h"

// We no longer need to inherit from component.  We just need
// initialise, update, render, and shutdown methods.
class ai_component
{
    // In reality you should still use PIMPL, but doesn't 
    // matter for this simple example.
private:
    entity _parent;
public:
    ai_component(entity parent)
    : _parent(parent)
    {
    }

    bool initialise()
    { 
        std::cout << "AI component initialised" << std::endl; 
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "AI component update" << std::endl;
    }

    void render()
    {
        std::cout << "AI component render" << std::endl;
    }

    void shutdown()
    {
        std::cout << "AI component shutdown" << std::endl;
    }
};