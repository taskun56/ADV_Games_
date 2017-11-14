#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <exception>
#include "component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// We will just define an entity as a collection of components.
class entity
{
    friend class entity_manager;
public:
    // A little dirty, but simplifies our definition.
    struct transform
    {
        glm::dmat4 Transform;

    };

private:
    // Counter to ID entities
    static size_t counter;

    struct entity_impl
    {
        size_t _id = 0;
        std::string _name = "";
        std::unordered_map<std::type_index, component> _components;

        transform trans;
    };
    
    std::shared_ptr<entity_impl> _self = nullptr;

    entity(const std::string &name) : _self{new entity_impl()}
    {
        _self->_id = counter++;
        _self->_name = name;
    }

public:

    entity() = default;

    transform& get_trans() { return _self->trans; }
	void set_trans(const glm::dmat4 m4) { _self->trans.Transform = m4; }

    bool initialise()
    {
        //std::cout << "Entity " << _self->_id << " initialising" << std::endl;
        // Call initialise on all components
        for (auto &c : _self->_components)
            if (!c.second.initialise())
                return false;
        return true;
    }

    bool load_content()
    {
        //std::cout << "Entity " << _self->_id << " loading content" << std::endl;
        // Call load_content on all components
        for (auto &c : _self->_components)
            if (!c.second.load_content())
                return false;
        return true;
    }

    void update(float delta_time)
    {
        //std::cout << "Entity " << _self->_id << " updating" << " " << _self->trans.x << std::endl;
        //std::cout << _self->_components.size() << std::endl;
        for (auto &c : _self->_components)
            c.second.update(delta_time);
    }

    void render()
    {
        //std::cout << "Entity " << _self->_id << " rendering" << std::endl;
        for (auto &c : _self->_components)
            c.second.render();
    }

    void unload_content()
    {
        //std::cout << "Entity " << _self->_id << " unloading content" << std::endl;
        for (auto &c : _self->_components)
            c.second.unload_content();
    }

    void shutdown()
    {
        //std::cout << "Entity " << _self->_id << " shutting down" << std::endl;
        for (auto &c : _self->_components)
            c.second.shutdown();
        _self->_components.clear();
    }

    template<typename T>
    bool add_component(T c, bool active = true, bool visible = true)
    {
        _self->_components[std::type_index(typeid(T))] = component{c, active, visible};
        return true;
    }

    template<typename T>
    T& get_component() 
    {
        auto found = _self->_components.find(std::type_index(typeid(T)));
        if (found != _self->_components.end())
        {
            return found->second.get<T>();
        }
        throw std::invalid_argument("Component not found");
    }
};

size_t entity::counter = 0;