#pragma once

#include <iostream>
#include <memory>
#include "component.h"

class ai_component : public component
{
protected:
    class ai_component_impl : public component::component_impl
    {
    public:
        ai_component_impl(entity &parent) : component::component_impl(parent) { }

        virtual bool initialise() override final
        { 
            std::cout << "AI component initialised" << std::endl; 
            return true;
        }

        virtual void update(float delta_time) override final
        {
            std::cout << "AI component update" << std::endl;
        }

        virtual void render() override final
        {
            std::cout << "AI component render" << std::endl;
        }

        virtual void shutdown() override final
        {
            std::cout << "AI component shutdown" << std::endl;
        }
    };

public:
    ai_component(entity &parent, bool active = true, bool visible = false)
    : component(std::make_shared<ai_component_impl>(parent), active, visible)
    {
    }
};