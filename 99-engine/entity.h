#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <exception>
#include "component.h"

// We will just define an entity as a collection of components.
class entity
{
    friend class entity_manager;
public:
    // A little dirty, but simplifies our definition.
    struct transform
    {
        float x = 1.0f;
        float y = 1.0f;
        float z = 1.0f;
    };

	//R//
	struct Scale
	{
		float x = 1.0f;
		float y = 1.0f;
		float z = 1.0f;
	};


	//R//Bounding circle (hitbox)	--Using SDL 29 circular collisions --Probably don't actually need this cause we could just set the radius and use the transform for the position
	struct circle
	{
		int x;
		int y;
		int z;
		int radius;
	};


	//R//enum to determine what the entity is - behaviours to be dictated by this -- May be declaring this in the wrong place
	enum entityType{Player, Enemy, Object};


private:
    // Counter to ID entities
    static size_t counter;

    struct entity_impl
    {
        size_t _id = 0;
        std::string _name = "";
        std::unordered_map<std::type_index, component> _components;

        transform trans;
		//R//
		Scale scale;

		//R//
		entityType et;
    };
    
    std::shared_ptr<entity_impl> _self = nullptr;

    entity(const std::string &name)	: _self{new entity_impl()}
    {
        _self->_id = counter++;
        _self->_name = name;
    }

public:

    entity() = default;

    transform& get_trans() 
	{ 
		return _self->trans; 
	}


	//R//Setter method for transform
	//R//To be used when spawning entities
	void set_trans(float x, float y, float z)
	{
		_self->trans.x = x;
		_self->trans.y = y;
		_self->trans.z = z;
	}


	//R//Getter method for scale
	Scale& get_scale()
	{
		return _self->scale;
	}


	//R//Setter method for scale
	void set_scale(float x, float y, float z)
	{
		_self->scale.x = x;
		_self->scale.y = y;
		_self->scale.z = z;
	}

	//R//Getter for entityType enum
	entityType& get_entityType()
	{
		return _self->et;
	}

	//R//Setter for entityType enum -- "Player" / "Enemy" / "Object"
	void set_entityType(std::string type)
	{
		if (type == "Player")
		{
			_self->et = entityType::Player;
		}
		if (type == "Enemy")
		{
			_self->et = entityType::Enemy;
		}
		if (type == "Object")
		{
			_self->et = entityType::Object;
		}
		else
		{
			//R//Throw an exception
			std::cout << "set_entityType recieved something other than the following:" << std::endl << "'Player'" << std::endl << "'Enemy'" << std::endl << "'Object'" << std::endl << std::endl;
			//throw std::invalid_argument("Incorrect string sent to set_entityType()");
			//R//This is being called even though it works when I comment out the exception...
		}
	}

    bool initialise()
    {
        std::cout << "Entity " << _self->_id << " initialising" << std::endl;
        // Call initialise on all components
        for (auto &c : _self->_components)
            if (!c.second.initialise())
                return false;
        return true;
    }

    bool load_content()
    {
        std::cout << "Entity " << _self->_id << " loading content" << std::endl;
        // Call load_content on all components
        for (auto &c : _self->_components)
            if (!c.second.load_content())
                return false;
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "Entity " << _self->_id << " updating" << " " << _self->trans.x << std::endl;
        std::cout << _self->_components.size() << std::endl;
        for (auto &c : _self->_components)
            c.second.update(delta_time);
    }

    void render()
    {
        std::cout << "Entity " << _self->_id << " rendering" << std::endl;
        for (auto &c : _self->_components)
            c.second.render();
    }

    void unload_content()
    {
        std::cout << "Entity " << _self->_id << " unloading content" << std::endl;
        for (auto &c : _self->_components)
            c.second.unload_content();
    }

    void shutdown()
    {
        std::cout << "Entity " << _self->_id << " shutting down" << std::endl;
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
    const T& get_component() const
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