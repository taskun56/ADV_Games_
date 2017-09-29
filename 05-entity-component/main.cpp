#include <iostream>
#include <vector>
#include "component.h"
#include "entity.h"
#include "physics_component.h"
#include "ai_component.h"
#include "render_component.h"

using namespace std;

int main(int argc, char **argv)
{
    // Create a vector of 10 entities
    vector<entity> entities;
    for (size_t i = 0; i < 10; ++i)
    {
        entity e;
        e.add_component(render_component(e, false, true));
        e.add_component(physics_component(e, true, false));
        e.add_component(ai_component(e, true, false));
        entities.push_back(e);
    }
    
    // Initialise entities
    for (auto &e : entities)
        e.initialise();

    // Update entities
    for (auto &e : entities)
        e.update(1.0f);

    // Render entities
    for (auto &e : entities)
        e.render();

    // Shutdow entities
    for (auto &e : entities)
        e.shutdown();

    return 0;
}