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







struct PowerUp_data
{

public:

	bool active = false;
	glm::dvec3 position;


	glm::dvec3 get_pos() { return position; }
	void set_pos(const glm::dvec3 v3) { position = v3; }

};


struct PowerUp_component
{
private:

	PowerUp_data *_data;

	entity &_parent;



public:


	PowerUp_component(entity &e, PowerUp_data *data) : _parent(e), _data(data)
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

class PowerUp_System : public singleton<PowerUp_System>, public factory<PowerUp_component, std::string, entity&>
{
	friend class singleton<PowerUp_System>;
private:
	struct PowerUp_impl
	{
		std::vector<PowerUp_data*> _data;
	};

	std::shared_ptr<PowerUp_impl> _self = nullptr;

	PowerUp_System() : _self{ new PowerUp_impl() }
	{
		register_constructor("Shield", [this](entity &e) { return this->build_component(e); });
	}

public:
	PowerUp_component build_component(entity &e)
	{
		_self->_data.push_back(new PowerUp_data());

		return PowerUp_component(e, _self->_data.back());
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





