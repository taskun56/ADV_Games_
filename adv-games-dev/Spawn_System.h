#pragma once
#include <time.h>
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







struct Spawn_data
{

public:

	bool active = false;
	int enemies;
	int powerups;
	float enemyTime;
	float powerUpTime;

};


struct Spawn_component
{
private:

	Spawn_data *_data;

	entity &_parent;

	

	entity Enemy[1000];
	entity PowerUp[1000];

public:


	Spawn_component(entity &e, Spawn_data *data) : _parent(e), _data(data)
	{
		_data->active = true;

		/* initialize random seed: */
		srand(time(NULL));

	}

	bool initialise()
	{

		return true;
	}


	void SpawnEnemy()
	{

		Enemy[_data->enemies] = entity_manager::get().create("ENTITY", "Enemy" + std::to_string(_data->enemies));
		Enemy[_data->enemies].add_component<physics_component>(physics_system::get().create("RIGID", Enemy[_data->enemies], glm::dvec3(0.0, 0.0, 0.0), glm::dquat(0.0, 0.0, 0.0, 0.0), glm::dvec3(0.50, 1.0, 1.0)));
		Enemy[_data->enemies].add_component<render_component>(renderer::get().create("REER", Enemy[_data->enemies], "PlayerShip.obj", "basic", 1));
		Enemy[_data->enemies].add_component<Enemy_component>(Enemy_System::get().create("BasicEnemy", Enemy[_data->enemies]));

		_data->enemies++;
	}



	void SpawnPowerUp()
	{

		PowerUp[_data->powerups] = entity_manager::get().create("ENTITY", "PowerUp" + std::to_string(_data->powerups));
		PowerUp[_data->powerups].add_component<physics_component>(physics_system::get().create("RIGID", PowerUp[_data->powerups], glm::dvec3(0.0, 0.0, 0.0), glm::dquat(0.0, 0.0, 0.0, 0.0), glm::dvec3(0.50, 1.0, 1.0)));
		PowerUp[_data->powerups].add_component<render_component>(renderer::get().create("REER", PowerUp[_data->powerups], "PlayerShip.obj", "basic", 1));
		PowerUp[_data->powerups].add_component<PowerUp_component>(PowerUp_System::get().create("Shield", PowerUp[_data->powerups]));

		_data->powerups++;
	}

	

	bool load_content()
	{
		return true;
	}

	void update(float delta_time)
	{
		_data->enemyTime += delta_time;
		_data->powerUpTime += delta_time;

		
		int randomNumber = rand() % 10 + 1;

		if (_data->enemyTime == 5.0f)
		{
			SpawnEnemy();
			_data->enemyTime = 0.0f;
		}
		else if(randomNumber == 5)
		{
			SpawnEnemy();
		}
		else
		{

		}

		if (_data->powerUpTime == 10.0f)
		{
			SpawnPowerUp();
			_data->enemyTime = 0.0f;
		}
		else if (randomNumber == 5)
		{
			SpawnPowerUp();
		}
		else
		{

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

class Spawn_System : public singleton<Spawn_System>, public factory<Spawn_component, std::string, entity&>
{
	friend class singleton<Spawn_System>;
private:
	struct Spawn_impl
	{
		std::vector<Spawn_data*> _data;
	};

	std::shared_ptr<Spawn_impl> _self = nullptr;

	Spawn_System() : _self{ new Spawn_impl() }
	{
		register_constructor("Spawn", [this](entity &e) { return this->build_component(e); });
	}

public:
	Spawn_component build_component(entity &e)
	{
		_self->_data.push_back(new Spawn_data());

		return Spawn_component(e, _self->_data.back());
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
	}
};




