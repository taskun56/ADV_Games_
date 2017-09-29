#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "component.h"

// entity is the same definition as last time except when
// the component is added.  We wrap the passed in parameter
// with a component.

class entity
{
private:
    // Used to index the entities.
    static size_t counter;

    // Use PIMPL.  entity should not be derived from so keep private.
    struct entity_impl
    {
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
    bool add_component(T c, bool active = true, bool visible = true)
    {
        _self->_components[std::type_index(typeid(T))] = component{c, active, visible};
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