#pragma once

#include "entity.h"
#include <iostream>

static size_t counter = 0;

bool entity::initialise()
{
    std::cout << "Entity " << _self->_id << " initialising" << std::endl;
    // Call initialise on all components
    for (auto &c : _self->_components)
        if (!c.second.initialise())
            return false;
    return true;
}

bool entity::load_content()
{
    std::cout << "Entity " << _self->_id << " loading content" << std::endl;
    // Call load_content on all components
    for (auto &c : _self->_components)
        if (!c.second.load_content())
            return false;
    return true;
}

void entity::update(float delta_time)
{
    //std::cout << "Entity " << _self->_id << " updating" << " " << _self->trans.x << std::endl;
    //std::cout << _self->_components.size() << std::endl;
    for (auto &c : _self->_components)
        c.second.update(delta_time);
}

void entity::render()
{
   std::cout << "Entity " << _self->_id << " rendering" << std::endl;
    for (auto &c : _self->_components)
        c.second.render();
}

void entity::unload_content()
{
    //std::cout << "Entity " << _self->_id << " unloading content" << std::endl;
    for (auto &c : _self->_components)
        c.second.unload_content();
}

void entity::shutdown()
{
    //std::cout << "Entity " << _self->_id << " shutting down" << std::endl;
    for (auto &c : _self->_components)
        c.second.shutdown();
    _self->_components.clear();
}
