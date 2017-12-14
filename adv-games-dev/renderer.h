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
#include "OpenGLRender.h"

struct render_data
{
	glm::dmat4 Transform;
	bool visible = false;
	GFX::Mesh *mesh;
	GFX::Shader *shade;
	int flag;
};

struct render_component
{
private:
	render_data *_data;

	entity *_parent;

	entity Test;

public:
	render_component(entity *e, render_data *data) : _parent(e), _data(data)
	{
		_data->visible = true;
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
		// This should never be called.
	}

	void render()
	{
		if (_data->visible)
		{


			//sets transform
			_data->Transform = _parent->get_trans().Transform;
			
			
			
		}
	}

	void unload_content()
	{
		_data->visible = false;
		_parent->set_active(_data->visible);
	}

	void shutdown()
	{

	}
};

class renderer : public singleton<renderer>, public factory<render_component, std::string, entity&, std::string, std::string, int>
{
	friend class singleton<renderer>;
private:
	struct renderer_impl
	{
		std::vector<render_data*> _data;
	};

	std::shared_ptr<renderer_impl> _self = nullptr;

	renderer() : _self{ new renderer_impl() }
	{
		register_constructor("RENDER", [this](entity &e, std::string shape, std::string shader, int state) { return this->build_component(e, shape, shader, state); });
		register_constructor("REER", [this](entity &e, std::string shape, std::string shader, int state) { return this->build_component(e, shape, shader, state); });
	}

public:
	render_component build_component(entity &e, std::string shape, std::string shader, int state)
	{
		_self->_data.push_back(new render_data());
		_self->_data.back()->mesh = GFX::GetMesh(shape);
		_self->_data.back()->shade = GFX::GetShaders(shader);
		_self->_data.back()->flag = state;
		return render_component(&e, _self->_data.back());
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
		// Should never be called
		//std::cout << "Renderer updating" << std::endl;
	}

	void render()
	{
		for (auto &d : _self->_data)
		{
			if (d->flag == 0)
			{
				d->visible = false;
			}
			if (d->visible)
			{

				glm::dmat4 PV = Camera_data::ActiveCam_->getVP();
				const glm::dmat4 MVP = PV * d->Transform;
				GLRender(d->mesh, d->shade, MVP);
			}
		}
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