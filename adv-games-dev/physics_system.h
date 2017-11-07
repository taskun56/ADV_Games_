#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "singleton.h"
#include "factory.h"
#include "entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

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

	glm::dmat4 Test;

    // We'll also keep a reference to the parent entity
    entity &_parent;
public:
    physics_component(entity &e, physics_data *data) : _parent(e), _data(data)
    {
        _data->active = true;
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
		/*
        // We will just update the entity position.
        _parent.get_trans().x = _data.x;
        _parent.get_trans().y = _data.y;
        _parent.get_trans().z = _data.z;
		*/


		

		transform = glm::translate(_data->Position) * glm::mat4_cast(_data->Rotation) * glm::scale(_data->Scale);
		_parent.set_trans(transform);
		//_parent.get_trans().Transform = transform;

		
//		Test = _parent.get_trans().Transform;

		
		
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


				d->Position = glm::dvec3(d->Position.x + 0.01f, d->Position.y, d->Position.z);
			//	d.Rotation = glm::dquat(d.Rotation.x + 0.01, d.Rotation.y, d.Rotation.z, d.Rotation.w);
	
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