#pragma once

#include "Camera_System.h"

Camera_data *Camera_data::ActiveCam_ = nullptr;

bool Camera_component::initialise()
{
	return true;
}

bool Camera_component::load_content()
{
	return true;
}

void Camera_component::update(float delta_time)
{
	PositionX.x = PositionX.x + 0.1;
	_data->SetPositionX(PositionX);

	_data->ViewMatrix = glm::lookAt(
		glm::dvec3(PositionX.x, 50, 1), // Camera is at (Player, 20, 1), in World Space
		glm::dvec3(PositionX.x, 0, 0), // and looks at the origin
		glm::dvec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

void Camera_component::render()
{

}

void Camera_component::unload_content()
{

}

void Camera_component::shutdown()
{

}

bool Camera_System::initialise()
{
	//std::cout << "Renderer initialising" << std::endl;
	return true;
}

bool Camera_System::load_content()
{
	//std::cout << "Renderer loading content" << std::endl;
	return true;
}

void Camera_System::update(float delta_time)
{

}

void Camera_System::render()
{

}

void Camera_System::unload_content()
{
	//std::cout << "Renderer unloading content" << std::endl;
}

void Camera_System::shutdown()
{
	//std::cout << "Renderer shutting down" << std::endl;
	for (auto &d : _self->_data)
	{
		delete d;
	}
}