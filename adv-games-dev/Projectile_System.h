#pragma once

#include <iostream>
#include <vector>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include <functional>
#include <string>
#include <sstream>
#include "singleton.h"
#include "factory.h"
#include "entity.h"







struct Projectile_data
{

public:

	bool active = false;
	glm::dvec3 position;
	int type;
	std::string Player = "Player";
	std::string Enemy = "Enemy";

	glm::dvec3 get_pos() { return position; }
	void set_pos(const glm::dvec3 v3) { position = v3; }

};


struct Projectile_component
{
private:

	Projectile_data *_data;

	entity *_parent;



public:


	Projectile_component(entity *e, Projectile_data *data) : _parent(e), _data(data)
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

		_data->set_pos(_parent->get_component<physics_component>().get_pos());


	

		if (_parent->get_name().compare(0, 6, _data->Player) == 0)
		{
			_data->set_pos(glm::dvec3(_data->position.x + 0.5f, _data->position.y, _data->position.z));
		}
		else
		{
			_data->set_pos(glm::dvec3(_data->position.x - 0.5f, _data->position.y, _data->position.z));
		}
		

		_parent->get_component<physics_component>().set_pos(_data->get_pos());

		//delete 
		if (_data->position.x < (Camera_data::ActiveCam_->PositionX.x - 30))
		{
			_parent->get_component<physics_component>().unload_content();
			_parent->get_component<render_component>().unload_content();
			_parent->get_component<Projectile_component>().unload_content();
		}

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

class Projectile_System : public singleton<Projectile_System>, public factory<Projectile_component, std::string, entity&>
{
	friend class singleton<Projectile_System>;
private:
	struct Projectile_impl
	{
		std::vector<Projectile_data*> _data;
	};

	std::shared_ptr<Projectile_impl> _self = nullptr;

	Projectile_System() : _self{ new Projectile_impl() }
	{
		register_constructor("BasicProjectile", [this](entity &e) { return this->build_component(e); });
	}

public:
	Projectile_component build_component(entity &e)
	{
		_self->_data.push_back(new Projectile_data());

		return Projectile_component(&e, _self->_data.back());
	}

	bool initialise()
	{
		//std::cout << "Renderer initialising" << std::endl;
		return true;
	}

	bool load_content()
	{
		//std::cout << "Renderer loading content" << std::endl;
		return true;
	}

	void update(float delta_time)
	{

	}

	void render()
	{

	}

	void unload_content()
	{
		//std::cout << "Renderer unloading content" << std::endl;
	}

	void shutdown()
	{
		//std::cout << "Renderer shutting down" << std::endl;
		for (auto &d : _self->_data)
		{
			delete d;
		}
	}
};





