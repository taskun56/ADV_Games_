#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "factory.h"

// Define a simple object to return
struct rigid_body
{
    float x;
    float y;
    float z;
};

// A function to construct a rigid body
rigid_body create_body()
{
    std::cout << "Called create body" << std::endl;
    rigid_body body;
    body.x = 0.0f;
    body.y = 10.0f;
    body.z = 20.0f;
    return body;
}

// Our actual factory
class physics_system : public factory<rigid_body, std::string> 
{
private:
    std::vector<rigid_body> _bodies;
public:
    physics_system()
    {
        register_constructor("FUN", create_body);
        // This binds this instances build method to a fuction.
        // Static functions don't require this.
        register_constructor("METHOD", std::bind(&physics_system::build, this));
    }

    rigid_body build()
    {
        std::cout << "Called method" << std::endl;
        rigid_body body;
        _bodies.push_back(body);
        return body;
    }
};