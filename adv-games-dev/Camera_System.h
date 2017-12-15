#pragma once

#include <iostream>
#include <vector>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include <functional>
#include <string>
#include <sstream>
#include "entity.h"
#include "singleton.h"
#include "factory.h"

struct Camera_data
{
public:

	bool active = false;
	glm::dmat4 ProjMatrix;
	glm::dmat4 ViewMatrix;
	glm::dvec3 PositionX;

	static Camera_data *ActiveCam_; 
	
	Camera_data::Camera_data()
	{
		ProjMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			glm::dvec3(0, 50, 1), // Camera is at (Player, 20, 1), in World Space
			glm::dvec3(0, 0, 0), // and looks at the origin
			glm::dvec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
	
	void SetActive() { ActiveCam_ = this; }
	glm::dmat4 Camera_data::getVP() const
	{
		return ProjMatrix * ViewMatrix;
	}

	void SetPositionX(const glm::dvec3 v3)
	{
		PositionX = v3;
	}
	
};


struct Camera_component
{
private:
	Camera_data *_data;

	entity &_parent;

	glm::dvec3 PositionX;

public:
	
	Camera_component(entity &e, Camera_data *data) : _parent(e), _data(data)
	{
		_data->active = true;
		_data->SetActive();
	}

	bool initialise();

	bool load_content();

	void update(float delta_time);

	void render();

	void unload_content();

	void shutdown();
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

	bool initialise();

	bool load_content();

	void update(float);

	void render();

	void unload_content();

	void shutdown();
};