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







struct Enemy_data
{

public:

	bool active = false;
	int Health;
	glm::dvec3 position;
	int Type;
	int WType;

	glm::dvec3 get_pos() { return position; }
	void set_pos(const glm::dvec3 v3) { position = v3; }

};


struct Enemy_component
{
private:

	Enemy_data *_data;

	entity &_parent;



public:


	Enemy_component(entity &e, Enemy_data *data) : _parent(e), _data(data)
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

		return Enemy_component(e, _self->_data.back());
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




