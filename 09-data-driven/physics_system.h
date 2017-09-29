#pragma once

#include <vector>

// We'll keep it simple, but look up the following for memory
// optimised solutions.
// * alignas
// * aligned_storage
struct physics_data
{
    bool active = false;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct physics_component
{
    physics_data &data;

    physics_component(physics_data &data) : data(data) { }
};

class physics_system
{
private:
    // A vector is normally as good as an array, but you can test.
    std::vector<physics_data> _data;

    size_t next = 0;
public:
    physics_system()
    : _data(1 << 10)
    {
    }

    physics_component create()
    {
        // You should do error checking on this.
        physics_component c(_data[next++]);
        c.data.active = true;
        return c;
    }

    void update()
    {
        std::cout << "Physics update" << std::endl;
        size_t n = 0;
        for (auto &e : _data)
        {
            if (e.active)
                std::cout << "Simulating " << n++ << std::endl;
        }
    }
};