#pragma once

#include <iostream>
#include <vector>
#include <time.h>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include <functional>
#include <string>
#include <sstream>
#include "singleton.h"
#include "factory.h"
#include "entity.h"

struct Enemy_data
{

public:

	bool active = false;
	int Health = 0;
	glm::dvec3 position;
	int Type;
	int WType;
	float enemyTime;
	int shots;
	float move;

	glm::dvec3 get_pos() { return position; }
	void set_pos(const glm::dvec3 v3) { position = v3; }
	void set_move(float m) { move = m; }

};

struct Enemy_component
{
private:

	Enemy_data *_data;

	entity *_parent;

	entity shot[1000];

public:


	Enemy_component(entity *e, Enemy_data *data) : _parent(e), _data(data)
	{

		/* initialize random seed: */
		srand(time(NULL));

		int randomNumber = rand() % 2;

		if (randomNumber == 1)
		{
			_data->set_move(0.1f);
		}
		else
		{
			_data->set_move(-0.1f);
		}
		_data->active = true;
	}

	bool initialise()
	{
		return true;

	}

	void shoot()
	{
		_data->shots++;

		shot[_data->shots] = entity_manager::get().create("ENTITY", "EnemyShot" + _parent->get_name() + std::to_string(_data->shots));
		shot[_data->shots].add_component<physics_component>(physics_system::get().create("RIGID", shot[_data->shots], glm::dvec3(_data->position.x, 0.0, _data->position.z), glm::dquat(0.0, 0.0, 0.0, 0.0), glm::dvec3(0.50, 1.0, 1.0)));
		shot[_data->shots].add_component<render_component>(renderer::get().create("REER", shot[_data->shots], "Bullet.obj", "basic", 1));
		shot[_data->shots].add_component<Projectile_component>(Projectile_System::get().create("BasicProjectile", shot[_data->shots]));
	}

	void damage()
	{
		_data->Health -= 1;

		//R//
		//std::cout << "Doing Damage to an enemy" << std::endl << std::endl;

		if (_data->Health < 0)
		{
			//std::cout << "RIP enemy bruh" << std::endl << std::endl << std::endl;
			_data->active = false;
		}
	}


	bool load_content()
	{
		return true;
	}

	void update(float delta_time)
	{
		//R//Trying to hack damage dealing, nothing to see here
		if (_parent->getDamageBool() == true)
		{
			damage();
			//std::cout << "Damage being dealt to an enemy" << std::endl << std::endl << std::endl << std::endl;
		}

		_parent->setDamageBool(false);


		_data->set_pos(_parent->get_component<physics_component>().get_pos());

		if (_data->position.z > (Camera_data::ActiveCam_->PositionX.z + 17))
		{
			_data->move = -0.1f;
		}
		if (_data->position.z < (Camera_data::ActiveCam_->PositionX.z - 17))
		{
			_data->move = 0.1f;

		}


		_data->set_pos(glm::dvec3(_data->position.x, _data->position.y, _data->position.z + _data->move));


		int randomNumber = rand() % 200 + 1;

		if (randomNumber == 5)
		{
			shoot();

		}





		_parent->get_component<physics_component>().set_pos(_data->get_pos());

		//delete
		if (_data->position.x < (Camera_data::ActiveCam_->PositionX.x - 30))
		{
			_parent->get_component<physics_component>().unload_content();
			_parent->get_component<render_component>().unload_content();
			_parent->get_component<Enemy_component>().unload_content();
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

class Enemy_System : public singleton<Enemy_System>, public factory<Enemy_component, std::string, entity&>
{
	friend class singleton<Enemy_System>;
private:
	struct Enemy_impl
	{
		std::vector<Enemy_data*> _data;
	};

	std::shared_ptr<Enemy_impl> _self = nullptr;

	Enemy_System() : _self{ new Enemy_impl() }
	{
		register_constructor("BasicEnemy", [this](entity &e) { return this->build_component(e); });
	}

public:
	Enemy_component build_component(entity &e)
	{
		_self->_data.push_back(new Enemy_data());
		return Enemy_component(&e, _self->_data.back());
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




