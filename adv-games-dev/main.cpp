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
	cout << "Main Menu State Entered." << endl;
}

void do_state_1(engine &eng)
{
	cout << "Operating Main Menu." << endl;
}

void exit_state_1(engine &eng)
{
	cout << "Main Menu Exit. Either through enter gameplay or exit application." << endl;
}

void enter_state_2(engine &eng)
{
	cout << "Gameplay State Entered." << endl;
}

void do_state_2(engine &eng)
{
	cout << "Continue to play Gameplay State." << endl;
}

void exit_state_2(engine &eng)
{
	cout << "Exiting Gameplay state. Either paused for menus or quit to Main Menu state or exit application." << endl;
}

void enter_state_3(engine &eng)
{
	cout << "State not implemented. State 3 Enter." << endl;
}

void do_state_3(engine &eng)
{
	cout << "State not implemented. State 3 DO-THING." << endl;
}

void exit_state_3(engine &eng)
{
	cout << "State not implemented. State 3 Exit." << endl;
}

void enter_state_4(engine &eng)
{
	cout << "Exit Game Called. Game should now quit." << endl;
	eng.set_running(false);
}

void do_state_4(engine &eng)
{
	cout << "State not implemented. State 4 DO-THING." << endl;
}

void exit_state_4(engine &eng)
{
	cout << "State not implemented. State 4 Exit." << endl;
}

int main(int arg, char **argv)
{
    auto eng = engine::get();
	eng.init();

    eng.add_state("MAIN", enter_state_1, do_state_1, exit_state_1);
	eng.add_state("GAME", enter_state_2, do_state_2, exit_state_2);
	eng.add_state("PAUSED", enter_state_3, do_state_3, exit_state_3);
	eng.add_state("EXIT", enter_state_4, do_state_4, exit_state_4);

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
    e.add_component<render_component>(renderer::get().create("RENDER", e, "PlayerShip.obj", "basic", 1));

	auto f = entity_manager::get().create("ENTITY", "buttsbuttsbutts");

	f.add_component<physics_component>(physics_system::get().create("RIGID", f));
	f.add_component<render_component>(renderer::get().create("RENDER", f, "EnemyShip2.obj", "basic", 1));

	if(eng.get_joystick_status()) eng.get_subsystem<input_handler>().InitializeJoysticks();

    eng.run();
    
    return 0;
}