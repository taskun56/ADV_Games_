#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "component.h"

class entity
{
private:
    // Used to index the entities.
    static size_t counter;

    // Use PIMPL.  entity should not be derived from so keep private.
    struct entity_impl
    {
        // A map (or dictionary) of components.
        // We use type as the key.  This makes life easier in many regards.
        // If you have multiple components of the same type, want to look up by name, 
        // or some other method (Run Time Type Information - RTTI - in C++ is "expensive")
        std::unordered_map<std::type_index, component> _components;
    };

    std::shared_ptr<entity_impl> _self = nullptr;
    
    size_t _my_id = 0;

public:

    entity()
    : _self{std::make_shared<entity_impl>()}
    {
        _my_id = counter++;
    }

    template<typename T>
    bool add_component(T c)
    {
        _self->_components[std::type_index(typeid(T))] = c;
        return true;
    }

    template<typename T>
    const T& get_component() const
    {
        auto found = _self->_components.find(std::type_index(typeid(T)));
        if (found != _self->_components.end())
        {
            return found->second;
        }
        throw std::invalid_argument("Component not found");
    }

    bool initialise()
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
        {
            if (!c.second.initialise())
                return false;
        }
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
            c.second.update(delta_time);
    }

    void render()
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
            c.second.render();
    }

    bool shutdown()
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
        {
            c.second.shutdown();
        }
        return true;
    }
};

size_t entity::counter = 0;