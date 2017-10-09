#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include "singleton.h"
#include "factory.h"
#include "entity.h"

struct render_data
{
    bool visible = false;
    // Let's pretend this is a matrix that was built.
    std::string transform = "(0, 0, 0)";
    std::string colour = "Red";
    std::string shape = "Sphere";
    std::string shader = "Phong";
};

struct render_component
{
private:
    render_data &_data;

    entity &_parent;
	//R//std::shared_ptr<entity> _parent;
public:
    render_component(entity &e, render_data &data) : _parent(e), _data(data)
    {
		this->_parent = e;		//J//
		//R//_active = false
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
            std::stringstream ss;
            ss << "(" << _parent.get_trans().x << ", " << _parent.get_trans().y << ", " << _parent.get_trans().z << ")" << std::endl;
            _data.transform = ss.str();
        }
    }

    void unload_content()
    {

    }

    void shutdown()
    {

    }
};

class renderer : public singleton<renderer>, public factory<render_component, std::string, entity&, std::string, std::string, std::string>
{
    friend class singleton<renderer>;
private:
    struct renderer_impl
    {
        std::vector<render_data> _data;
    };

    std::shared_ptr<renderer_impl> _self = nullptr;

    renderer()	: _self{new renderer_impl()}
    {
        register_constructor("RENDER", [this](entity &e, std::string colour, std::string shape, std::string shader) 
		{ 
			return this->build_component(e, colour, shape, shader); 
		});
    }

public:
    render_component build_component(entity &e, std::string colour, std::string shape, std::string shader)
    {
		
        _self->_data.push_back(render_data());
        _self->_data.back().colour = colour;
        _self->_data.back().shape = shape;
        _self->_data.back().shader = shader;
        return render_component(e, _self->_data.back());
    }

    bool initialise()
    {
		
        std::cout << "Renderer initialising" << std::endl;
        return true;
    }

    bool load_content()
    {
        std::cout << "Renderer loading content" << std::endl;
        return true;
    }

    void update(float delta_time)
    {
        // Should never be called
        std::cout << "Renderer updating" << std::endl;
    }

    void render()
    {
        std::cout << "Renderer rendering" << std::endl;
        for (auto &d : _self->_data)
        {
            if (d.visible)
            {
                std::cout << "Rendering " << d.colour << " ";
                std::cout << d.shape << " using " << d.shader;
                std::cout << " shading at position " << d.transform << std::endl;
            }
        }
    }

    void unload_content()
    {
        std::cout << "Renderer unloading content" << std::endl;
    }

    void shutdown()
    {
        std::cout << "Renderer shutting down" << std::endl;
    }
};