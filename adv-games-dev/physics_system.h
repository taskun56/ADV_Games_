#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "singleton.h"
#include "factory.h"
#include "entity.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/quaternion.hpp"
#include "../include/glm/gtx/euler_angles.hpp"
#include "../include/glm/gtx/transform.hpp"

struct physics_data
{
    bool active = false;
    // This is position
	glm::dvec3 Position;
	glm::dquat Rotation;
	glm::dvec3 Scale;

};

struct physics_component
{
private:
    // We'll just keep a reference here.  The physics system
    // will maintain the actual data.
    physics_data *_data;
	
	glm::dmat4 transform;

    // We'll also keep a reference to the parent entity
    entity *const _parent;
	entity Test;

public:
    physics_component(entity &e, physics_data *data) : _parent(&e), _data(data)
    {
        _data->active = true;
		
    }
	physics_component() = delete;

	glm::dvec3 get_pos() { return _data->Position; }
	void set_pos(const glm::dvec3 v3) { _data->Position = v3; }


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
		for (auto iterator = entity_manager::get()._self->_entities.begin(); iterator != entity_manager::get()._self->_entities.end(); iterator++)
		{
			//std::cout << iterator->second._self->_name << " collider updating..." << std::endl << std::endl;
			iterator->second.update_collider(iterator->second.get_trans().Transform);
		}
	}



	void DetectCollisions()
	{
		//R//Find the player (to compare against everything else for collisions
		//auto player = _self->_entities.find("ob1");// ->second._self->;
		auto player = _parent;
		//R// For each entity in _entities

		for (auto iterator = entity_manager::get()._self->_entities.begin(); iterator != entity_manager::get()._self->_entities.end(); iterator++)
		{
			//R//If the current entity in _entities IS the player
			//if (iterator->first == "ob1")
			if (iterator->second.get_name() == player->get_name())
			{
				//R//Print the name of the entity 
				//	std::cout << iterator->second._self->_name << std::endl;
				//	std::cout << iterator->second.get_trans().Transform[3].x << " " << iterator->second.get_trans().Transform[3].y << " " << iterator->second.get_trans().Transform[3].z << std::endl;
				//	//std::cout << "Found the player (ob1), skipping..." << std::endl << std::endl;
				//	std::cout << "Found the player (" << playername << "), skipping..." << std::endl << std::endl;

				//R//Skip that loop cycle and go to the next entity in _entities
				continue;
			}

			//R//If the current entity in _entities is the camera
			if (iterator->first == "Camera")
			{
				//R//Print the name of the entity 
				//	std::cout << iterator->second._self->_name << std::endl;
				//	std::cout << iterator->second.get_trans().Transform[3].x << " " << iterator->second.get_trans().Transform[3].y << " " << iterator->second.get_trans().Transform[3].z << std::endl;
				//	std::cout << "Found the camera, skipping..." << std::endl << std::endl;

				//R//Skip that loop cycle and go to the next entity in _entities
				continue;
			}


			bool bewl = false;

			//R//Enemies can collide freely with each other and enemy projectiles with out issues
			if (_parent->get_name().find("Enemy") == 0 && iterator->first.find("Enemy" == 0))
			{
				//std::cout << "parent name starts with Enemy and so does what it collided with" << std::endl << std::endl;

				continue;
			}
			//R//Player can collide with its own bullets freely without issues 
			if (_parent->get_name().find("Player") == 0 && iterator->first.find("Player") == 0)
			{
				std::cout << "parent name starts with Player and so does what it collided with" << std::endl << std::endl;

				continue;
			}






			//R//Collision Detection
			//R//player circle a, iterator->second._self circle b
			bool check = checkCollision(_parent->get_collider(), iterator->second.get_collider());
			if (check == true)
			{
				//std::cout << player->second._self->_name << " collided with " << iterator->second._self->_name << std::endl << std::endl;

				//if (_parent->get_name().find("Enemy") == 0)
				if (iterator->second.get_name().find("Enemy") == 0)
				{
					//_parent->get_component<Enemy_component>().damage();
					bewl = true;
					//_parent->setDamageBool(bewl);
					_parent->setDamageBool(bewl);
					iterator->second.setDamageBool(bewl);
				}
				//if (_parent->get_name().find("Player") == 0)
				if (iterator->second.get_name().find("Player") == 0)
				{
					bewl = true;
					_parent->setDamageBool(bewl);
					iterator->second.setDamageBool(bewl);
					//_parent->get_component<Player_component>().damage();
				}
			}

			//player->second._self->

			//R//Print the name of the entity 
			//std::cout << iterator->second._self->_name << ": ";
			//std::cout << iterator->second.get_trans().Transform[3].x << " " << iterator->second.get_trans().Transform[3].y << " " << iterator->second.get_trans().Transform[3].z << std::endl;
			//std::cout << "collider: " << iterator->second._self->collider.Transform[3].x << " " << iterator->second._self->collider.Transform[3].y << " " << iterator->second._self->collider.Transform[3].z << std::endl;
			//std::cout << "collider radius: " << iterator->second._self->collider.radius << std::endl << std::endl;

			//auto player = _self->_entities.find("playerplayer");
			//std::cout << player->second._self->_name << " has a position of " << player->second._self->trans.x << " " << player->second._self->trans.y << " " << player->second._self->trans.z << std::endl;

			//R//We should now use this to check for collisions
			//R//We need to find the player entity, and then cycle through the other entities and decide whether or not the player has collided with anything
			//R//We can then worry about a second player at a later stage

		}
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
		//std::cout << &_parent << std::endl;	
		transform = glm::translate(_data->Position) * glm::mat4_cast(_data->Rotation) * glm::scale(_data->Scale);

		//Collisions need to check for if player collides with anything   enemy ship and enemy projectiles will take off health of player and will then delete themselves can just make inactive using unloadcontent only problem is slow down as game goes on    powerUps will grant more health again make inactive  
		//Projectiles need to check who they collide with if name matches with there own do nothing       else damage then make projectile inactive
		//PowerUp

		_parent->set_trans(transform);	

		//R//
		update_all_colliders();
		DetectCollisions();
    }

    void render()
    {
    }

    void unload_content()
    {
		_data->active = false;
    }

    void shutdown()
    {

    }
};

class physics_system : public singleton<physics_system>, public factory<physics_component, std::string, entity&, glm::dvec3 , glm::dquat , glm::dvec3>
{
    friend class singleton<physics_system>;
private:
    struct physics_system_impl
    {
        // Maintain a vector of physics_data
		std::vector<physics_data*> _data;
    };



    physics_system()
    : _self{new physics_system_impl()}
    {
        register_constructor("RIGID", [this](entity &e, glm::dvec3 pos, glm::dquat rot, glm::dvec3 scal){ return this->build_component(e, pos, rot, scal); });
    }

public:
	std::shared_ptr<physics_system_impl> _self = nullptr;


    physics_component build_component(entity &e, glm::dvec3 pos, glm::dquat rot, glm::dvec3 scal)
    {
        _self->_data.push_back(new physics_data());
		_self->_data.back()->Position = pos;
		_self->_data.back()->Scale = scal;
		_self->_data.back()->Rotation = rot;
        return physics_component(e, _self->_data.back());
    }


    bool initialise()
    {
        //std::cout << "Physics system initialising" << std::endl;
        return true;
    }

    bool load_content()
    {
        //std::cout << "Physics system loading content" << std::endl;
        return true;
    }

    void update(float delta_time)
    {
        //std::cout << "Physics system updating" << std::endl;
        for (auto &d : _self->_data)
        {
            // Updates the entity so change pos rot and scale here   
            if (d->active)
            {
			//	d->Position = glm::dvec3(d->Position.x + 0.1f, d->Position.y , d->Position.z);
			//	d->Rotation = glm::dquat(d->Rotation.x + 0.0001, d->Rotation.y, d->Rotation.z, d->Rotation.w);
	
            }
        }



		
    }



    void render()
    {
        // This should never be called.
        //std::cout << "Physics system rendering" << std::endl;
    }

    void unload_content()
    {
        //std::cout << "Physics system unloading content" << std::endl;
    }

    void shutdown()
    {
        //std::cout << "Physics system shutting down" << std::endl;
		for (auto &d : _self->_data)
		{
			delete d;
		}
    }

};