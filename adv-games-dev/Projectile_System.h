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







struct Projectile_data
{

public:

	bool active = false;
	glm::dvec3 position;
	int type;


	glm::dvec3 get_pos() { return position; }
	void set_pos(const glm::dvec3 v3) { position = v3; }

};


struct Projectile_component
{
private:

	Projectile_data *_data;

	entity &_parent;



public:


	Projectile_component(entity &e, Projectile_data *data) : _parent(e), _data(data)
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

		_parent.get_trans();
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

		return Projectile_component(e, _self->_data.back());
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





