#include <iostream>
#include "engine.h"
#include "input_handler.h"
#include "entity_manager.h"
#include "physics_system.h"
#include "renderer.h"


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


	//R//Messing around with how to create a player entity
	auto Player1 = entity_manager::get().create("ENTITY", "playerplayer");
	Player1.set_trans(0.0f, 0.0f, 0.0f);
	cout << "Player 1 exists and is located at X/Y/Z:" << endl;
	cout << Player1.get_trans().x << "/" << Player1.get_trans().y << "/" << Player1.get_trans().z << endl << endl;
	Player1.add_component<physics_component>(physics_system::get().create("RIGID", Player1));
	Player1.add_component<render_component>(renderer::get().create("RENDER", Player1, string("Blue"), "Box", 1));

	cout << "entityType: " << Player1.get_entityType() << endl << endl;
	Player1.set_entityType("Enemy");
	cout << "entityType: " << Player1.get_entityType() << endl << endl;
	
	auto Player2 = entity_manager::get().create("ENTITY", "Barker");		Player2.set_trans(2, 2, 2);
	auto Player3 = entity_manager::get().create("ENTITY", "Murray");		Player3.set_trans(3, 3, 3);
	auto Player4 = entity_manager::get().create("ENTITY", "Boyle");			Player4.set_trans(4, 4, 4);
	auto Player5 = entity_manager::get().create("ENTITY", "McGinn");		Player5.set_trans(5, 5, 5);
	auto Player6 = entity_manager::get().create("ENTITY", "Bartley");		Player6.set_trans(6, 6, 6);
	auto Player7 = entity_manager::get().create("ENTITY", "McGeouch");		Player7.set_trans(7, 7, 7);
	auto Player8 = entity_manager::get().create("ENTITY", "Stevenson");		Player8.set_trans(8, 8, 8);
	auto Player9 = entity_manager::get().create("ENTITY", "Hanlon");		Player9.set_trans(9, 9, 9);
	auto Player10 = entity_manager::get().create("ENTITY", "Ambrose");		Player10.set_trans(10, 10, 10);
	auto Player11 = entity_manager::get().create("ENTITY", "Whittaker");	Player11.set_trans(11, 11, 11);
	auto Player12 = entity_manager::get().create("ENTITY", "Marciano");		Player12.set_trans(12, 12, 12);

	//R//Testing cycling through _entities
	entity_manager::get().CycleThroughEntities();


	if(eng.get_joystick_status()) eng.get_subsystem<input_handler>().InitializeJoysticks();

    eng.run();
    
    return 0;
}