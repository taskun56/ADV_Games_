#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "entity.h"
#include "singleton.h"
#include "factory.h"

// entity manager just holds our entities in a lookup table.
// This is a simple system - you might want more.
class entity_manager : public singleton<entity_manager>, public factory<entity, std::string, std::string>
{
    friend class singleton<entity_manager>;
private:
    struct entity_manager_impl
    {
        std::unordered_map<std::string, entity> _entities;
    };

    std::shared_ptr<entity_manager_impl> _self = nullptr;

    entity_manager()
    : _self{new entity_manager_impl()} 
    { 
        register_constructor("ENTITY", [this](std::string name)
		{ 
			return this->create_entity(name); 
		});
    }

public:

    entity create_entity(std::string name)
    {
        _self->_entities[name] = entity(name);
        return _self->_entities.at(name);
    }

    bool initialise()
    {
        std::cout << "Entity manager initialising" << std::endl;
        for (auto &e : _self->_entities)
            if (!e.second.initialise())
                return false;
        return true;
    }

    bool load_content()
    {
        std::cout << "Entity manager loading content" << std::endl;
        for (auto &e : _self->_entities)
            if (!e.second.load_content())
                return false;
            return true;
    }

    void update(float delta_time)
    {
        std::cout << "Entity manager updating" << std::endl;
        for (auto &e : _self->_entities)
            e.second.update(delta_time);
    }

    void render()
    {
        std::cout << "Entity manager rendering" << std::endl;
        for (auto &e : _self->_entities)
            e.second.render();
    }

    void unload_content()
    {
        std::cout << "Entity manager unloading content" << std::endl;
        for (auto &e : _self->_entities)
            e.second.unload_content();
    }

    void shutdown()
    {  
         std::cout << "Entity manager shutting down" << std::endl;
         for (auto &e : _self->_entities)
            e.second.shutdown();
        // Clear the entity map
        _self->_entities.clear();
    }
};