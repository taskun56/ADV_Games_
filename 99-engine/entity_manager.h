#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "entity.h"
#include "singleton.h"
#include "factory.h"

// entity manager just holds our entities in a lookup table.
// This is a simple system - you might want more.
class entity_manager : public singleton<entity_manager>, public factory<entity, std::string, std::string>
{
    friend class singleton<entity_manager>;
private:
    struct entity_manager_impl
    {
        std::unordered_map<std::string, entity> _entities;
    };

    std::shared_ptr<entity_manager_impl> _self = nullptr;

    entity_manager()	: _self{new entity_manager_impl()} 
    { 
        register_constructor("ENTITY", [this](std::string name)
		{ 
			return this->create_entity(name); 
		});
    }

public:

    entity create_entity(std::string name)
    {
        _self->_entities[name] = entity(name);
        return _self->_entities.at(name);
    }

    bool initialise()
    {
        std::cout << "Entity manager initialising" << std::endl;
        for (auto &e : _self->_entities)
            if (!e.second.initialise())
                return false;
        return true;
    }

    bool load_content()
    {
        std::cout << "Entity manager loading content" << std::endl;
        for (auto &e : _self->_entities)
            if (!e.second.load_content())
                return false;
            return true;
    }

    void update(float delta_time)
    {
        std::cout << "Entity manager updating" << std::endl;
        for (auto &e : _self->_entities)
            e.second.update(delta_time);
    }

    void render()
    {
        std::cout << "Entity manager rendering" << std::endl;
        for (auto &e : _self->_entities)
            e.second.render();
    }

    void unload_content()
    {
        std::cout << "Entity manager unloading content" << std::endl;
        for (auto &e : _self->_entities)
            e.second.unload_content();
    }

    void shutdown()
    {  
         std::cout << "Entity manager shutting down" << std::endl;
         for (auto &e : _self->_entities)
            e.second.shutdown();
        // Clear the entity map
        _self->_entities.clear();
    }






	//R//Squaring the distance
	double SquaredDistance(int x1, int y1, int x2, int y2)
	{
		int deltaX = x2 - x1;
		int deltaY = y2 - y1;

		return deltaX*deltaX + deltaY*deltaY;
	}

	//R//Check for circle vs circle collisions
	bool checkCollision(entity::circle& a, entity::circle& b)
	{
		//Calculate total radius squared
		int SquaredTotalRadius = a.radius + b.radius;
		SquaredTotalRadius *= SquaredTotalRadius;

		//If the distance between the centers of the circles is less than the sum of their radii
		if (SquaredDistance(a.x, a.y, b.x, b.y) < (SquaredTotalRadius))
		{
			//The circles have collided
			return true;
		}
		return false;
	}

	//R//Check for circle vs SDL_Rect collisions -- commented out for now as I don't have SDL set up
	/*
	bool checkCollision(entity::circle& a, SDL_Rect& b)
	{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (a.x < b.x)
	{
	cX = b.x;
	}
	else if (a.x > b.x + b.w)
	{
	cX = b.x + b.w;
	}
	else
	{
	cX = a.x;
	}

	//Find closest y offset
	if (a.y < b.y)
	{
	cY = b.y;
	}
	else if (a.y > b.y + b.h)
	{
	cY = b.y + b.h;
	}
	else
	{
	cY = a.y;
	}

	//If the closest point is inside the circle
	if (SquaredDistance(a.x, a.y, cX, cY) < a.r * a.r)
	{
	//This box and the circle have collided
	return true;
	}
	return false;
	}*/




	void CycleThroughEntities()
	{
		//R// For each entity in _entities
		for (auto iterator = _self->_entities.begin(); iterator != _self->_entities.end(); iterator++)
		{
			//R//Print the name of the entity 
			std::cout << iterator->second._self->_name << std::endl;

			//R//This we should now use this to check for collisions 
			//R//We need to find the player entity, and then cycle through the other entities and decide whether or not the player has collided with anything
			//R//We can then worry about a second player at a later stage
		}
	}

};