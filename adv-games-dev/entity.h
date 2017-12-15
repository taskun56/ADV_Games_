#pragma once

#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <exception>
#include "component.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

// We will just define an entity as a collection of components.
static size_t counter = 0;
class entity
{
    friend class entity_manager;
public:
    
    struct transform
    {
        glm::dmat4 Transform;
    };

	//R//Bounding circle (hitbox)	--Using SDL 29 circular collisions --Probably don't actually need this cause we could just set the radius and use the transform for the position
	struct circle
	{
		glm::dmat4 Transform;
		float radius = 2;
	};

private:

	

    struct entity_impl
    {
        size_t _id = 0;
        std::string _name = "";
        std::unordered_map<std::type_index, component> _components;
		bool active = true;
        transform trans;
		circle collider;

		//R// trying to hack damage dealing, nothing to see here
		bool dirtyhackycheatz = false;
    };
    
    std::shared_ptr<entity_impl> _self = nullptr;

    entity(const std::string &name) : _self{new entity_impl()}
    {
        _self->_id = counter++;
        _self->_name = name;
    }

public:

    entity() = default;

	void set_active(bool Alive) { _self->active = Alive; }
	std::string& get_name() { return _self->_name; }
    transform& get_trans() { return _self->trans; }
	circle& get_collider() { return _self->collider; }
	void set_trans(const glm::dmat4 m4) { _self->trans.Transform = m4; }

	//R//setup collider radius
	void setColliderSize(float rad)
	{
		_self->collider.radius = rad;
	}

	//void update_collider(glm::dmat4 Transform)
	void update_collider(glm::dmat4 Transform)
	{
		_self->collider.Transform = Transform;
	}

	//R// trying to hack damage dealing, nothing to see here
	void setDamageBool(bool booly)
	{
		if (booly)
		{
			std::cout << "True" << std::endl;
			_self->dirtyhackycheatz = true;
		}
		else
		{
			//std::cout << "false" << std::endl;
			_self->dirtyhackycheatz = false;
		}
	}
	//R// trying to hack damage dealing, nothing to see here
	bool getDamageBool()
	{
		return _self->dirtyhackycheatz;
	}


	bool initialise();

	bool load_content();

	void update(float delta_time);

	void render();

	void unload_content();

	void shutdown();

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
