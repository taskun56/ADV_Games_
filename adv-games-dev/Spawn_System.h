#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "entity.h"
#include "singleton.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// entity manager just holds our entities in a lookup table.
// This is a simple system - you might want more.
class Spawn_System : public singleton<Spawn_System>
{

private:


public:



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
		//std::cout << "Spawn manager updating" << std::endl;
	
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