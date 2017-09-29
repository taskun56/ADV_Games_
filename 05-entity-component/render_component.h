#pragma once

#include <iostream>
#include <memory>
#include "component.h"

class render_component : public component
{
protected:
    class render_component_impl : public component::component_impl
    {
    public:
        render_component_impl(entity &parent) : component::component_impl(parent) { }

        virtual bool initialise() override final
        { 
            std::cout << "Render component initialised" << std::endl; 
            return true;
        }

        virtual void update(float delta_time) override final
        {
            std::cout << "Render component update" << std::endl;
        }

        virtual void render() override final
        {
            std::cout << "Render component render" << std::endl;
        }

        virtual void shutdown() override final
        {
            std::cout << "Render component shutdown" << std::endl;
        }
    };

public:
    render_component(entity &parent, bool active = false, bool visible = true)
    : component(std::make_shared<render_component_impl>(parent), active, visible)
    {
    }
};