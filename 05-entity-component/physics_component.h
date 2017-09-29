#pragma once

#include <iostream>
#include <memory>
#include "component.h"

class physics_component : public component
{
protected:
    class physics_component_impl : public component::component_impl
    {
    public:
        physics_component_impl(entity &parent) : component::component_impl(parent) { }

        virtual bool initialise() override final
        { 
            std::cout << "Physics component initialised" << std::endl; 
            return true;
        }

        virtual void update(float delta_time) override final
        {
            std::cout << "Physics component update" << std::endl;
        }

        virtual void render() override final
        {
            std::cout << "Physics component render" << std::endl;
        }

        virtual void shutdown() override final
        {
            std::cout << "Physics component shutdown" << std::endl;
        }
    };

public:
    physics_component(entity &parent, bool active = true, bool visible = false)
    : component(std::make_shared<physics_component_impl>(parent), active, visible)
    {
    }
};