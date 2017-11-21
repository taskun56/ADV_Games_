#pragma once
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







struct Camera_data
{

public:

	bool active = false;
	glm::dmat4 ProjMatrix;
	glm::dmat4 ViewMatrix;
	static Camera_data *ActiveCam_; 
	

	Camera_data::Camera_data()
	{
		ProjMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			glm::dvec3(0, 20, 1), // Camera is at (Player, 20, 1), in World Space
			glm::dvec3(0, 0, 0), // and looks at the origin
			glm::dvec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
	
	void SetActive() { ActiveCam_ = this; }
	glm::dmat4 Camera_data::getVP() const
	{
		return ProjMatrix * ViewMatrix;
	}
	
};


struct Camera_component
{
private:
	Camera_data *_data;

	entity &_parent;

	glm::dvec3 hi;

public:
	

	Camera_component(entity &e, Camera_data *data) : _parent(e), _data(data)
	{
		_data->active = true;
		_data->SetActive();
		
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
		
		

		auto his = Player_data::ActivePlayer_->get_pos();

		
		hi = his;

	//	Player_data::GetActivePlayer().position;

		_data->ViewMatrix = glm::lookAt(
			glm::dvec3(hi.x, 20, 1), // Camera is at (Player, 20, 1), in World Space
			glm::dvec3(hi.x, 0, 0), // and looks at the origin
			glm::dvec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	
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

class Camera_System : public singleton<Camera_System>, public factory<Camera_component, std::string, entity&>
{
	friend class singleton<Camera_System>;
private:
	struct Camera_impl
	{
		std::vector<Camera_data*> _data;
	};

	std::shared_ptr<Camera_impl> _self = nullptr;

	Camera_System() : _self{ new Camera_impl() }
	{
		register_constructor("Camera", [this](entity &e) { return this->build_component(e); });
	}

public:
	Camera_component build_component(entity &e)
	{
		_self->_data.push_back(new Camera_data());
		
		return Camera_component(e, _self->_data.back());
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

Camera_data *Camera_data::ActiveCam_ = nullptr;