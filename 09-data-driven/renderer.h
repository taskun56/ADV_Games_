#pragma once

#include <vector>

// We'll keep it simple, but look up the following for memory
// optimised solutions.
// * alignas
// * aligned_storage
struct render_data
{
    bool visible = false;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct render_component
{
    render_data &data;

    render_component(render_data &data) : data(data) { }
};

class renderer
{
private:
    // A vector is normally as good as an array, but you can test.
    std::vector<render_data> _data;

    size_t next = 0;
public:
    renderer()
    : _data(1 << 10)
    {
    }

    render_component create()
    {
        // You should do error checking on this.
        render_component c(_data[next++]);
        c.data.visible = true;
        return c;
    }

    void render()
    {
        std::cout << "Render called" << std::endl;
        size_t n = 0;
        for (auto &e : _data)
        {
            if (e.visible)
                std::cout << "Rendering " << n++ << std::endl;
        }
    }
};