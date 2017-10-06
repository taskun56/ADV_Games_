#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "component.h"

class entity
{
private:
    // Used to index the entities.
    static size_t counter;

    // Use PIMPL.  entity should not be derived from so keep private.
    struct entity_impl
    {
        // A map (or dictionary) of components.
        // We use type as the key.  This makes life easier in many regards.
        // If you have multiple components of the same type, want to look up by name, 
        // or some other method (Run Time Type Information - RTTI - in C++ is "expensive")
        std::unordered_map<std::type_index, component> _components;								//R//To access this - entity_impl [name]->_components
    };

	//R//Seems to create a shared_ptr called _self but then set it to null
    std::shared_ptr<entity_impl> _self = nullptr;
    
	
    size_t _my_id = 0;

public:

	//R//Not sure what the colon is for but it breaks without it
    entity()	: _self{std::make_shared<entity_impl>()} //R//Seems to turn _self back into a shared_ptr of entity_impl once an entity is initialised
    {
		//R//Think counter is actually being saved as counter++, as well as setting the id to it.. not sure though
        _my_id = counter++;
    }


    template<typename T>	//R//Typename T means that you're not declaring what type of input you're expecting but the compiler can handle it and work it out itself.
    bool add_component(T c)	//R//Component c is going to be passed in, but we're not telling the compiler what type the component is of. 
    {
        _self->_components[std::type_index(typeid(T))] = c;		//R//Accessing _components of _self and setting the typeid of T to c..? 
        return true;
    }

    template<typename T>
    const T& get_component() const	//R//T& returns a reference to T, and is const because we don't want to alter the reference. 
									//R//the const after the method name bans the method from attempting to alter any member variables in the object.
    {
        auto found = _self->_components.find(std::type_index(typeid(T)));
        if (found != _self->_components.end())
        {
            return found->second;
        }
        throw std::invalid_argument("Component not found");
    }

    bool initialise()
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
        {
            if (!c.second.initialise())
                return false;
        }
        return true;
    }

    void update(float delta_time)
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
            c.second.update(delta_time);
    }

    void render()
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
            c.second.render();
    }

    bool shutdown()
    {
        std::cout << "entity " << _my_id << std::endl;
        for (auto &c : _self->_components)
        {
            c.second.shutdown();
        }
        return true;
    }
};

size_t entity::counter = 0;