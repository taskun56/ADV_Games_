#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	int Health;
	static Player_data *ActivePlayer_;

	static Player_data &GetActivePlayer() { static Player_data activePlayer_{ *ActivePlayer_ }; return activePlayer_; }
	void SetActive() { ActivePlayer_ = this; }

};


struct Player_component
{
private:

	Player_data &_data;

	entity &_parent;



public:


	Player_component(entity &e, Player_data &data) : _parent(e), _data(data)
	{
		_data.active = true;
		_data.SetActive();

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

class Player_System : public singleton<Player_System>, public factory<Player_component, std::string, entity&>
{
	friend class singleton<Player_System>;
private:
	struct Player_impl
	{
		std::vector<Player_data> _data;
	};

	std::shared_ptr<Player_impl> _self = nullptr;

	Player_System() : _self{ new Player_impl() }
	{
		register_constructor("Player", [this](entity &e) { return this->build_component(e); });
	}

public:
	Player_component build_component(entity &e)
	{
		_self->_data.push_back(Player_data());

		return Player_component(e, _self->_data.back());
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

Player_data *Player_data::ActivePlayer_ = nullptr;