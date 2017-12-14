#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "singleton.h"
#include "factory.h"
#include "entity.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/quaternion.hpp"
#include "../include/glm/gtx/euler_angles.hpp"
#include "../include/glm/gtx/transform.hpp"

struct physics_data
{
    bool active = false;
    // This is position
	glm::dvec3 Position;
	glm::dquat Rotation;
	glm::dvec3 Scale;

};

struct physics_component
{
private:
    // We'll just keep a reference here.  The physics system
    // will maintain the actual data.
    physics_data *_data;
	
	glm::dmat4 transform;

    // We'll also keep a reference to the parent entity
    entity *const _parent;
	entity Test;

public:
    physics_component(entity &e, physics_data *data) : _parent(&e), _data(data)
    {
        _data->active = true;
		
    }
	physics_component() = delete;

	glm::dvec3 get_pos() { return _data->Position; }
	void set_pos(const glm::dvec3 v3) { _data->Position = v3; }

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
		//std::cout << &_parent << std::endl;	
		transform = glm::translate(_data->Position) * glm::mat4_cast(_data->Rotation) * glm::scale(_data->Scale);

		//Collisions need to check for if player collides with anything   enemy ship and enemy projectiles will take off health of player and will then delete themselves can just make inactive using unloadcontent only problem is slow down as game goes on    powerUps will grant more health again make inactive  
		//Projectiles need to check who they collide with if name matches with there own do nothing       else damage then make projectile inactive
		//PowerUp

		_parent->set_trans(transform);	


    }

    void render()
    {
    }

    void unload_content()
    {
		_data->active = false;
    }

    void shutdown()
    {

    }
};

class physics_system : public singleton<physics_system>, public factory<physics_component, std::string, entity&, glm::dvec3 , glm::dquat , glm::dvec3>
{
    friend class singleton<physics_system>;
private:
    struct physics_system_impl
    {
        // Maintain a vector of physics_data
        std::vector<physics_data*> _data;
    };

    std::shared_ptr<physics_system_impl> _self = nullptr;

    physics_system()
    : _self{new physics_system_impl()}
    {
        register_constructor("RIGID", [this](entity &e, glm::dvec3 pos, glm::dquat rot, glm::dvec3 scal){ return this->build_component(e, pos, rot, scal); });
    }

public:



    physics_component build_component(entity &e, glm::dvec3 pos, glm::dquat rot, glm::dvec3 scal)
    {
        _self->_data.push_back(new physics_data());
		_self->_data.back()->Position = pos;
		_self->_data.back()->Scale = scal;
		_self->_data.back()->Rotation = rot;
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
            // Updates the entity so change pos rot and scale here   
            if (d->active)
            {
			//	d->Position = glm::dvec3(d->Position.x + 0.1f, d->Position.y , d->Position.z);
			//	d->Rotation = glm::dquat(d->Rotation.x + 0.0001, d->Rotation.y, d->Rotation.z, d->Rotation.w);
	
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
		for (auto &d : _self->_data)
		{
			delete d;
		}
    }

};