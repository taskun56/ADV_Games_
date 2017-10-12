#pragma once

#include <iostream>
#include "singleton.h"
#include "entity.h"
#include <string>

// This class is purely code so we don't worry about
// PIMPL here.
class input_handler : public singleton<input_handler>
{
    friend class singleton<input_handler>;
private:
    input_handler() = default;
public:
    bool initialise()
    {
        std::cout << "Input handler initialising" << std::endl;
        return true;
    }

    bool load_content()
    {
        std::cout << "Input handler loading content" << std::endl;
        return true;
    }

    void update(float delta_time)
    {
        // We are really cheating here.  You should use standard key down 
        // checking each frame here.
        std::cout << "Input handler updating" << std::endl;
        int choice = 1;
        std::cout << "Change state to 1-4: ";
        
        engine::get().change_state(std::to_string(choice), engine::get());
    }

    void render()
    {
        // This should never be called.
        std::cout << "Input handler rendering" << std::endl;
    }

    void unload_content()
    {
        std::cout << "Input handler unloading content" << std::endl;
    }

    void shutdown()
    {
        std::cout << "Input handler shutting down" << std::endl;
    }
};