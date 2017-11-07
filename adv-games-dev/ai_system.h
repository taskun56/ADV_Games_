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







struct AI_data
{

public:

	bool active = false;
	glm::dmat4 ProjMatrix;
	glm::dmat4 ViewMatrix;
	static AI_data *ActiveCam_; 
	

	AI_data::AI_data()
	{
		ProjMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			glm::dvec3(5, 6, 3), // Camera is at (4,3,3), in World Space
			glm::dvec3(0, 0, 0), // and looks at the origin
			glm::dvec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
	
	static AI_data &GetActiveCam() { static AI_data activeCam_{*ActiveCam_}; return activeCam_; }
	void SetActive() { ActiveCam_ = this; }
	glm::dmat4 AI_data::getVP() const
	{
		return ProjMatrix * ViewMatrix;
	}
	
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

AI_data *AI_data::ActiveCam_ = nullptr;