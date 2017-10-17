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


struct render_data
{
	bool visible = false;
	Mesh *mesh;
	Shader *shade;
	int flag;
};

struct render_component
{
private:
	render_data &_data;

	entity &_parent;
public:
	render_component(entity &e, render_data &data) : _parent(e), _data(data)
	{
		_data.visible = true;
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
		if (_data.visible)
		{
			// "Generate" the transform matrix.
			//std::cout << "Rendering component of entity " << std::endl;
			//std::stringstream ss;
			//ss << "(" << _parent.get_trans().x << ", " << _parent.get_trans().y << ", " << _parent.get_trans().z << ")" << std::endl;
			//_data.transform = ss.str();
		}
	}

	void unload_content()
	{

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
		std::vector<render_data> _data;
	};

	std::shared_ptr<renderer_impl> _self = nullptr;

	renderer() : _self{ new renderer_impl() }
	{
		register_constructor("RENDER", [this](entity &e, std::string shape, std::string shader, int state) { return this->build_component(e, shape, shader, state); });
	}

public:
	render_component build_component(entity &e, std::string shape, std::string shader, int state)
	{
		_self->_data.push_back(render_data());
		_self->_data.back().mesh = GetMesh(shape);
		_self->_data.back().shade = GetShaders(shader);
		_self->_data.back().flag = state;
		return render_component(e, _self->_data.back());
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
			if (d.flag == 0)
			{
				d.visible = false;
			}
			if (d.visible)
			{
				glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
				// Camera matrix
				glm::mat4 View = glm::lookAt(
					glm::vec3(5, 6, 3), // Camera is at (4,3,3), in World Space
					glm::vec3(0, 0, 0), // and looks at the origin
					glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
				);
				// Model matrix : an identity matrix (model will be at the origin)
				glm::mat4 Model = glm::mat4(1.0f);


				const glm::mat4 MVP = Projection * View * Model;
				GLRender(d.mesh, d.shade, MVP);
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
	}
};