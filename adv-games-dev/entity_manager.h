#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "entity.h"
#include "singleton.h"
#include "factory.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

// entity manager just holds our entities in a lookup table.
// This is a simple system - you might want more.
class entity_manager : public singleton<entity_manager>, public factory<entity, std::string, std::string>
{
    friend class singleton<entity_manager>;
private:


public:


	struct entity_manager_impl
	{
		std::unordered_map<std::string, entity> _entities;
	};

	std::shared_ptr<entity_manager_impl> _self = nullptr;

	entity_manager() : _self{ new entity_manager_impl() }
	{
		register_constructor("ENTITY", [this](std::string name) { return this->create_entity(name); });
	}

    entity create_entity(std::string name)
    {
        _self->_entities[name] = entity(name);
        return _self->_entities.at(name);
    }

    bool initialise()
    {
        //std::cout << "Entity manager initialising" << std::endl;
        for (auto &e : _self->_entities)
            if (!e.second.initialise())
                return false;
        return true;
    }

    bool load_content()
    {
        //std::cout << "Entity manager loading content" << std::endl;
        for (auto &e : _self->_entities)
            if (!e.second.load_content())
                return false;
            return true;
    }

    void update(float delta_time)
    {
        //std::cout << "Entity manager updating" << std::endl;
        for (auto &e : _self->_entities)
            e.second.update(delta_time);
    }

    void render()
    {
        //std::cout << "Entity manager rendering" << std::endl;
        for (auto &e : _self->_entities)
            e.second.render();
    }

    void unload_content()
    {
        //std::cout << "Entity manager unloading content" << std::endl;
        for (auto &e : _self->_entities)
            e.second.unload_content();
    }

    void shutdown()
    {  
         //std::cout << "Entity manager shutting down" << std::endl;
         for (auto &e : _self->_entities)
            e.second.shutdown();
        // Clear the entity map
        _self->_entities.clear();
    }







	//R//Squaring the distance
	double SquaredDistance(float x1, float y1, float x2, float y2)
	{
		float deltaX = x2 - x1;
		float deltaY = y2 - y1;

		return deltaX*deltaX + deltaY*deltaY;
	}

	//R//Check for circle vs circle collisions
	bool checkCollision(entity::circle& a, entity::circle& b)
	{
		//Calculate total radius squared
		int SquaredTotalRadius = a.radius + b.radius;
		SquaredTotalRadius *= SquaredTotalRadius;

		//If the distance between the centers of the circles is less than the sum of their radii
		if (SquaredDistance(a.Transform[3].x, a.Transform[3].z, b.Transform[3].x, b.Transform[3].z) < (SquaredTotalRadius))
		{
			//The circles have collided
			std::cout << "COLLISION DETECTED!" << std::endl;
			return true;
		}
		return false;
	}




	//R// - Currently just updates the colliders to match entitiy position, needs to be changed so that it depends on whether or not there was a collision
	void update_all_colliders()
	{
		//R// For each entity in _entities
		for (auto iterator = _self->_entities.begin(); iterator != _self->_entities.end(); iterator++)
		{
			std::cout << iterator->second._self->_name << " collider updating..." << std::endl << std::endl;
			iterator->second.update_collider(iterator->second._self->trans.Transform);
		}
	}



	void DetectCollisions(std::string playername)
	{
		//R//Find the player (to compare against everything else for collisions
		//auto player = _self->_entities.find("ob1");// ->second._self->;
		auto player = _self->_entities.find(playername);
		//R// For each entity in _entities
		for (auto iterator = _self->_entities.begin(); iterator != _self->_entities.end(); iterator++)
		{
			//R//If the current entity in _entities IS the player
			//if (iterator->first == "ob1")
			if (iterator->first == playername)
			{
				//R//Print the name of the entity 
				std::cout << iterator->second._self->_name << std::endl;
				std::cout << iterator->second.get_trans().Transform[3].x << " " << iterator->second.get_trans().Transform[3].y << " " << iterator->second.get_trans().Transform[3].z << std::endl;
				//std::cout << "Found the player (ob1), skipping..." << std::endl << std::endl;
				std::cout << "Found the player (" << playername << "), skipping..." << std::endl << std::endl;

				//R//Skip that loop cycle and go to the next entity in _entities
				continue;
			}

			//R//If the current entity in _entities is the camera
			if (iterator->first == "Camera")
			{
				//R//Print the name of the entity 
				std::cout << iterator->second._self->_name << std::endl;
				std::cout << iterator->second.get_trans().Transform[3].x << " " << iterator->second.get_trans().Transform[3].y << " " << iterator->second.get_trans().Transform[3].z << std::endl;
				std::cout << "Found the camera, skipping..." << std::endl << std::endl;

				//R//Skip that loop cycle and go to the next entity in _entities
				continue;
			}


			//R//Collision Detection
			//R//player circle a, iterator->second._self circle b
			bool check = checkCollision(player->second._self->collider, iterator->second._self->collider);
			if (check == true)
			{
				std::cout << player->second._self->_name << " collided with " << iterator->second._self->_name << std::endl << std::endl;
			}

			//player->second._self->

			//R//Print the name of the entity 
			std::cout << iterator->second._self->_name << ": ";
			std::cout << iterator->second.get_trans().Transform[3].x << " " << iterator->second.get_trans().Transform[3].y << " " << iterator->second.get_trans().Transform[3].z << std::endl;
			std::cout << "collider: " << iterator->second._self->collider.Transform[3].x << " " << iterator->second._self->collider.Transform[3].y << " " << iterator->second._self->collider.Transform[3].z << std::endl;
			std::cout << "collider radius: " << iterator->second._self->collider.radius << std::endl << std::endl;

			//auto player = _self->_entities.find("playerplayer");
			//std::cout << player->second._self->_name << " has a position of " << player->second._self->trans.x << " " << player->second._self->trans.y << " " << player->second._self->trans.z << std::endl;

			//R//We should now use this to check for collisions
			//R//We need to find the player entity, and then cycle through the other entities and decide whether or not the player has collided with anything
			//R//We can then worry about a second player at a later stage
		}
	}
};