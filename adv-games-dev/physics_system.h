#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "singleton.h"
#include "factory.h"
#include "entity.h"

struct physics_data
{
    bool active = false;
    // This is position
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct physics_component
{
private:
    // We'll just keep a reference here.  The physics system
    // will maintain the actual data.
    physics_data &_data;

    // We'll also keep a reference to the parent entity
    entity &_parent;
public:
    physics_component(entity &e, physics_data &data) : _parent(e), _data(data)
    {
        _data.active = true;
    }

    bool initialise()
    {

        return true;
    }

    bool load_content()
    {
        return true;
    }

    void update(float delta_time)
    {
        // We will just update the entity position.
        _parent.get_trans().x = _data.x;
        _parent.get_trans().y = _data.y;
        _parent.get_trans().z = _data.z;
    }

    void render()
    {
    }

    void unload_content()
    {
    }

    void shutdown()
    {

    }
};

class physics_system : public singleton<physics_system>, public factory<physics_component, std::string, entity>
{
    friend class singleton<physics_system>;
private:
    struct physics_system_impl
    {
        // Maintain a vector of physics_data
        std::vector<physics_data> _data;
    };

    std::shared_ptr<physics_system_impl> _self = nullptr;

    physics_system()
    : _self{new physics_system_impl()}
    {
        register_constructor("RIGID", [this](entity e){ return this->build_component(e); });
    }

public:
    physics_component build_component(entity e)
    {
        _self->_data.push_back(physics_data());
        return physics_component(e, _self->_data.back());
    }

    bool initialise()
    {
        std::cout << "Physics system initialising" << std::endl;
        return true;
    }

    bool load_content()
    {
        std::cout << "Physics system loading content" << std::endl;
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "Physics system updating" << std::endl;
        for (auto &d : _self->_data)
        {
            // If active physics object add 1 to each component.
            if (d.active)
            {
                d.x += 1.0f;
                d.y += 1.0f;
                d.z += 1.0f;
            }
        }
    }

    void render()
    {
        // This should never be called.
        std::cout << "Physics system rendering" << std::endl;
    }

    void unload_content()
    {
        std::cout << "Physics system unloading content" << std::endl;
    }

    void shutdown()
    {
        std::cout << "Physics system shutting down" << std::endl;
    }
};