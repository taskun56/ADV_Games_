#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "component.h"

struct transform
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
};

class entity
{
private:
    // Counter to ID entities
    static size_t counter;

    size_t _id = 0;
    std::string _name = "";
    std::unordered_map<std::string, std::shared_ptr<component>> _components;
    transform _trans;

public:
    entity(const std::string& name)	: _name(name) //R//The colon part is called an initialisation list. It allows you to change a const variable before the body of the construtor is executed.
    {
        _id = counter++;
    }

	//Get the transform struct of an entity
    transform& get_trans() 
	{ 
		return _trans; 
	}

    bool initialise()
    {
        std::cout << "Entity " << _id << " initialising" << std::endl;
       
		// Call initialise on all components
		for (auto &c : _components)
		{
			if (!c.second->initialise())
			{
				return false;
			}
		}
        return true;
    }

    bool load_content()
    {
        std::cout << "Entity " << _id << " loading content" << std::endl;
        
		// Call load_content on all components
		for (auto &c : _components)
		{
			if (!c.second->load_content())
			{
				return false;
			}
		}
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "Entity " << _id << " updating" << std::endl;
		std::cout << "Entity '" << _name << "' updating" << std::endl;		//R//
        std::cout << _components.size() << std::endl;
		
		//Call update on all components
		for (auto &c : _components)
		{
			c.second->update(delta_time);
		}
    }

    void render()
    {
        std::cout << "Entity " << _id << " rendering" << std::endl;
		
		//Call render on all components
		for (auto &c : _components)
		{
			c.second->render();
		}
    }

    void unload_content()
    {
        std::cout << "Entity " << _id << " unloading content" << std::endl;
		
		//Call unload_content on all components
		for (auto &c : _components)
		{
			c.second->unload_content();
		}
    }

    void shutdown()
    {
        std::cout << "Entity " << _id << " shutting down" << std::endl;
		
		//Call shutdown on all components
		for (auto &c : _components)
		{
			c.second->shutdown();
		}
        _components.clear(); //Clear the _components map
    }

    bool add_component(const std::string &name, std::shared_ptr<component> comp)
    {
		//Add a new component to the _components map
        _components[name] = comp;
        return true;
    }

    std::shared_ptr<component> get_component(const std::string &name) const
    {
		//Get component based on name 
        auto found = _components.find(name);
		if (found != _components.end())
		{
			return found->second;
		}
        return nullptr;
    }
};

size_t entity::counter = 0;