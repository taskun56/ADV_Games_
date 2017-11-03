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




struct Camera
{
private:
	glm::dmat4 ProjMatrix;
	glm::dmat4 ViewMatrix;
	static Camera *activeCam_;
	
	

public:

	Camera::Camera()
		:ProjMatrix(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f)),
		ViewMatrix(glm::lookAt(glm::vec3(5, 6, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))) {}

	void Camera::SetActive() { activeCam_ = this; }
	static Camera &Camera::GetActiveCam() { return *activeCam_; }

	glm::dmat4 Camera::getVP() const
	{
		return ProjMatrix * ViewMatrix;
	}

};


struct AI_data
{
	bool active = false;
	static Camera _Camera;
};


struct AI_component
{
private:
	AI_data &_data;

	entity &_parent;
public:
	AI_component(entity &e, AI_data &data) : _parent(e), _data(data)
	{
		_data.active = true;
	}

	bool initialise()
	{
		_data._Camera.SetActive();

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

class AI_System : public singleton<AI_System>, public factory<AI_component, std::string, entity&>
{
	friend class singleton<AI_System>;
private:
	struct AI_impl
	{
		std::vector<AI_data> _data;
	};

	std::shared_ptr<AI_impl> _self = nullptr;

	AI_System() : _self{ new AI_impl() }
	{
		register_constructor("Camera", [this](entity &e) { return this->build_component(e); });
	}

public:
	AI_component build_component(entity &e)
	{
		_self->_data.push_back(AI_data());
		
		return AI_component(e, _self->_data.back());
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

