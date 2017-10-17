#include <iostream>
#include "engine.h"
#include "input_handler.h"
#include "entity_manager.h"
#include "physics_system.h"
#include "renderer.h"
#include "../include/SDL.h"

using namespace std;

void enter_state_1(engine &eng)
{

}

void do_state_1(engine &eng)
{

}

void exit_state_1(engine &eng)
{

}

void enter_state_2(engine &eng)
{

}

void do_state_2(engine &eng)
{

}

void exit_state_2(engine &eng)
{

}

void enter_state_3(engine &eng)
{

}

void do_state_3(engine &eng)
{

}

void exit_state_3(engine &eng)
{

}

void enter_state_4(engine &eng)
{

}

void do_state_4(engine &eng)
{

}

void exit_state_4(engine &eng)
{

}

bool init(engine &eng)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Check if there is at least one controller attached 
		if (SDL_NumJoysticks() < 1)
		{
			printf("No joysticks connected.");
		}
		else
		{
			eng.get_subsystem<input_handler>().initialise();
		}
	}

	return success;
}

int main(int arg, char **argv)
{
    auto eng = engine::get();

    eng.add_state("1", enter_state_1, do_state_1, exit_state_1);
    eng.add_state("2", enter_state_2, do_state_2, exit_state_2);
    eng.add_state("3", enter_state_3, do_state_3, exit_state_3);
    eng.add_state("4", enter_state_4, do_state_4, exit_state_4);

    // Input handler does not render
    eng.add_subsystem(input_handler::get(), true, false);
    // Entity manager has to update and render
    eng.add_subsystem(entity_manager::get(), true, true);
    // Physics system does not render
    eng.add_subsystem(physics_system::get(), true, false);
    // Renderer does not update.
    eng.add_subsystem(renderer::get(), false, true);

    auto e = entity_manager::get().create("ENTITY", "Test");
    //e.get_trans().x;
    e.add_component<physics_component>(physics_system::get().create("RIGID", e));
    e.add_component<render_component>(renderer::get().create("RENDER", e, string("Blue"), "Box", "Physical"));

	auto f = entity_manager::get().create("ENTITY", "buttsbuttsbutts");

	f.add_component<physics_component>(physics_system::get().create("RIGID", f));
	f.add_component<render_component>(renderer::get().create("RENDER", f, string("Blue"), "Box", "Physical"));

	init(eng);

    eng.run();
    
    return 0;
}