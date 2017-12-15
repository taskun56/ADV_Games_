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





struct Player_data
{

public:

	bool active = false;
	bool Invincible = false;
	float InvincibleTimer = 0.0f;
	int Health;
	int WType;
	int Score;
	static Player_data *ActivePlayer_;
    glm::dvec3 position;
	int shots;
	glm::dvec3 vel;
	bool shooting = false;

	void SetActive() { ActivePlayer_ = this; }
	glm::dvec3 get_pos() { return position; }
	void set_pos(const glm::dvec3 v3) { position = v3; }
	void set_WType(int W) { WType = W; }
	int get_Health() { return Health; }
	void set_Health(int H) { Health = H; }

};


struct Player_component
{
private:



	entity *_parent;

	entity shot[1000];
	

public:

	Player_data *_data;

	Player_component(entity *e, Player_data *data) : _parent(e), _data(data)
	{
		_data->active = true;
		_data->SetActive();
		_data->Invincible = true;
	}


	bool initialise()
	{
		return true;
	}

	void shoot()
	{
	
		shot[_data->shots] = entity_manager::get().create("ENTITY", "PlayerShot" + std::to_string(_data->shots));
		shot[_data->shots].add_component<physics_component>(physics_system::get().create("RIGID", shot[_data->shots], glm::dvec3(_data->position.x, 0.0, 0.0), glm::dquat(0.0, 0.0, 0.0, 0.0), glm::dvec3(0.50, 1.0, 1.0)));
		shot[_data->shots].add_component<render_component>(renderer::get().create("REER", shot[_data->shots], "Bullet.obj", "basic", 1));
		shot[_data->shots].add_component<Projectile_component>(Projectile_System::get().create("BasicProjectile", shot[_data->shots]));

		_data->shots++;
	}

	void move(glm::dvec3 vel)
	{
		_data->set_pos(glm::dvec3(_data->position.x + vel.x, _data->position.y + vel.y, _data->position.z + vel.z));
	}

	void damage()
	{
		if (_data->Invincible == false)
		{
			_data->Health - 1;
			_data->Invincible = true;

		}
	}


	bool load_content()
	{
		return true;
	}

	void update(float delta_time)
	{

		_data->set_pos(_parent->get_component<physics_component>().get_pos());

		if (_data->shooting)
		{
			shoot();
			_data->shooting = false;
		}
		

		if (_data->Invincible == true && _data->InvincibleTimer < 5.0f)
		{
			_data->InvincibleTimer += delta_time;
		}
		else
		{
			_data->Invincible = false;
			_data->InvincibleTimer = 0;
		}


		move(_data->vel);

		//
		////Right
		//if (SDL_GetKeyFromName("D"))
		//{
		//	_data->set_pos(glm::dvec3(_data->position.x + 0.1f, _data->position.y, _data->position.z));
		//}

		////Left
		//if (SDL_GetKeyFromName("A"))
		//{
		//	_data->set_pos(glm::dvec3(_data->position.x - 0.1f, _data->position.y, _data->position.z));
		//}

		////Up
		//if (SDL_GetKeyFromName("W"))
		//{
		//	_data->set_pos(glm::dvec3(_data->position.x, _data->position.y, _data->position.z - 0.1f));
		//}

		////Down
		//if (SDL_GetKeyFromName("S"))
		//{
		//	_data->set_pos(glm::dvec3(_data->position.x, _data->position.y, _data->position.z + 0.1f));
		//}

		//if (SDL_GetKeyFromName("Space"))
		//{
		//	shoot();
		//}

		

		//Make sure player isnt too far back
		if (_data->position.x < (Camera_data::ActiveCam_->PositionX.x - 30))
		{
			_data->set_pos(glm::dvec3(Camera_data::ActiveCam_->PositionX.x - 25, _data->position.y, _data->position.z));
		}

		//Make sure player isnt too far forward
		if (_data->position.x > (Camera_data::ActiveCam_->PositionX.x + 30))
		{
			_data->set_pos(glm::dvec3(Camera_data::ActiveCam_->PositionX.x + 25, _data->position.y, _data->position.z));
		}

		//Make sure player isnt too far up
		if (_data->position.z < (Camera_data::ActiveCam_->PositionX.z - 18))
		{
			_data->set_pos(glm::dvec3(_data->position.x, _data->position.y, Camera_data::ActiveCam_->PositionX.z - 13));
		}

		//Make sure player isnt too far down
		if (_data->position.z > (Camera_data::ActiveCam_->PositionX.z + 18))
		{
			_data->set_pos(glm::dvec3(_data->position.x, _data->position.y, Camera_data::ActiveCam_->PositionX.z + 13));
		}

		if (_data->get_Health() == 0)
		{
			//GameOver
			
			

		}

		
		

		_parent->get_component<physics_component>().set_pos(_data->get_pos());

		
		
		_data->vel = glm::dvec3();
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

class Player_System : public singleton<Player_System>, public factory<Player_component, std::string, entity&>
{
	friend class singleton<Player_System>;
private:
	struct Player_impl
	{
		std::vector<Player_data*> _data;
	};



	Player_System() : _self{ new Player_impl() }
	{
		register_constructor("Player", [this](entity &e) { return this->build_component(e); });
	}

public:
	std::shared_ptr<Player_impl> _self = nullptr;

	Player_component build_component(entity &e)
	{
		_self->_data.push_back(new Player_data());

		return Player_component(&e, _self->_data.back());
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

Player_data *Player_data::ActivePlayer_ = nullptr;

