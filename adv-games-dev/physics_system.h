#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "singleton.h"
#include "factory.h"
#include "entity.h"


//R// trying to use graphics bitbucket to figure out sine wave movement
float total_time = 0.0f;
float sine = 0.0f;


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

	//R//Getter for physics_data Jeremy asked for 
	physics_data& getData()
	{
		return _data;
	}

    void update(float delta_time)
    {
		//R//Trying to use graphics bitbucket to work out sine wave movement
		//Accumulate time
		total_time += delta_time;
		//Update sine based on sin wave
		sine = sinf(total_time);
		//Multiply by 5
		sine *= 5;


		/*
        // We will just update the entity position.
        _parent.get_trans().x = _data.x;
        _parent.get_trans().y = _data.y;
        _parent.get_trans().z = _data.z;
		*/


		//R//Checking what type of entity we are updating	-- Not sure _parent.[entityType] works
		//R//If entity = player
		if (_parent.get_entityType() == entity::entityType::Player)
		{
			//Move based on input stuff
		}

		//R//if entity = enemy
		if (_parent.get_entityType() == entity::entityType::Enemy)
		{
			//Determine what kind of enemy it is
			//Give it movement based on enemy types

			//Straight down enemy movement
			_parent.set_trans(_parent.get_trans().x, _parent.get_trans().y--, _parent.get_trans().z);

			//Sine wave enemy movement 
			//R//bb computer-graphics-cw1: s = 1.0f + sinf(total_time);
			//R//s is then used to scale something based on time. 
			//_parent.set_trans(_parent.get_trans().x * sine, _parent.get_trans().y--, _parent.get_trans().z);
		}

		//R//if entity = object
		if (_parent.get_entityType() == entity::entityType::Object)
		{
			//
		}
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
        //std::cout << "Physics system initialising" << std::endl;
        return true;
    }

    bool load_content()
    {
        //std::cout << "Physics system loading content" << std::endl;
        return true;
    }

    void update(float delta_time)
    {
        //std::cout << "Physics system updating" << std::endl;
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
        //std::cout << "Physics system rendering" << std::endl;
    }

    void unload_content()
    {
        //std::cout << "Physics system unloading content" << std::endl;
    }

    void shutdown()
    {
        //std::cout << "Physics system shutting down" << std::endl;
    }
};