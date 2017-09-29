#include <iostream>
#include <utility>
#include "physics_system.h"
#include "renderer.h"

using namespace std;

int main(int argc, char **argv)
{
    physics_system physics;
    renderer render;
    using entity = std::pair<physics_component, render_component>;
    vector<entity> entities;
    for (size_t i = 0; i < 10; ++i)
    {
        entities.push_back({physics.create(), render.create()});
    }
    physics.update();
    render.render();

    return 0;
}