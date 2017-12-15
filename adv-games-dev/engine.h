#pragma once

#include "input_handler.h"
#include "../include/gl/glew.h"
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <exception>
#include "subsystem.h"
#include "singleton.h"
#include "state_machine.h"
#include "../include/SDL.h"
#include "../include/SDL_opengl.h"
#include "../include/SDL_ttf.h"
#include <GL\GLU.h>
#include "Menu.cpp"
#include "Options.cpp"
#include "Controls.cpp"
#include "Resolution.cpp"
#include "GameOver.cpp"

//R//Need this to alter volume in game loop
#include "SoundSystem.h"
SoundSystem Sounds;


class engine : public singleton<engine>, public state_machine<engine>
{
    friend class singleton<engine>;
private:
    // engine_impl is the private implementation of engine.  Ensures that we only
    // copy a pointer when we create a new reference to the engine.  The engine will
    // be referenced by each subsystem.
    struct engine_impl
    {
        // The collection of subsystems.  Note the type <type_index, subsystem>.  type_index
        // is a unique identifier for a type.  THIS IS NOT NECESSARILY EFFICIENT.  If you
        // are interested look up RTTI (Run Time Type Information) in C++.  I use it here
        // to make life easier.
        // A BETTER method would be to define your own indexing or to use strings.  This
        // would allow multiple subsystems of the same type.
        std::unordered_map<std::type_index, subsystem> _subsystems;
    };

    // Pointer to the implementation.  This is shared as each subsystem will have a
    // reference to the engine.
    std::shared_ptr<engine_impl> _self = nullptr;

    // Flag to indicate if the engine is running or not.
    bool _running = true;

	// SDL Event for taking all events each window
	SDL_Event e;
	bool _joy = false;
	int x;
	int y;

    // Private constructor.  Called when we call get.
    engine() : _self(new engine_impl())
    {

    }

public:

	SDL_Window* gWindow = NULL;
	SDL_GLContext gContext;
	std::string state_set = "MAIN";
	std::string new_state_set;
	int state;
    // Adds a subsystem to the engine.
    template<typename T>
    void add_subsystem(T sys, bool active = true, bool visible = true)
    {
        // We aren't doing any checking here.  We are assuming that no other subsystem
        // of this type exists.  If it does, it will be overriden.
        // We use curly braces construction which is the currently advised method.
        // The curly braces could be replaced by normal parenthesis.
        _self->_subsystems[std::type_index(typeid(T))] = subsystem{sys, active, visible};
    }

    // Gets a subsystem from the engine.  We use the type to look up and then do
    // some template magic to convert it to the type we want by getting the internally
    // wrapped object.  We also return a reference to avoid copying.
    template<typename T>
    T& get_subsystem()
    {
        // This could be done in compressed lines.  I will do this a line at a time for
        // illustration purposes.
        // Find the system of the given type.
        auto found = _self->_subsystems.find(std::type_index(typeid(T)));
        // Found is either a valid value or the end of the collection.  Deal with the
        // latter.
        if (found == _self->_subsystems.end())
        {
            // No subsystem of type found.  Fail.
            throw std::invalid_argument("Failed to find subsystem");
        }
        // We know a subsystem of the type was found.  We need to return it as the
        // correct type.  Thankfully subsystem has code to help us.
        return found->second.get<T>();
    }

    // You might want to add code for removing subsystems.

    // Get the current running value.
    bool get_running() const noexcept { return _running; }

    // Set the current running value.
    void set_running(bool value) noexcept { _running = value; }



	int MainMenu()
	{
	
		Menu menu;

		state = menu.showmenu(SDL_GetWindowSurface(gWindow), e, gWindow);

		return state;

	}

	int OptionsMenu()
	{

		Options opt;

		state = opt.showoptions(SDL_GetWindowSurface(gWindow), e, gWindow);

		return state;

	}

	int ResolutionMenu()
	{

		Resolution res;

		state = res.showresolution(SDL_GetWindowSurface(gWindow), e, gWindow);

		return state;

	}

	int ControllerMenu()
	{

		Control con;

		state = con.showcontrols(SDL_GetWindowSurface(gWindow), e, gWindow);

		return state;

	}

	int GameOverMenu()
	{
		GameOver gam;

		state = gam.showmenu(SDL_GetWindowSurface(gWindow), e, gWindow);

		return state;
	}

	void Score()
	{


	}


	bool init()
	{
		//Initialization flag
		bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Use OpenGL 4.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			// Check if there is at least one controller attached 
			if (SDL_NumJoysticks() < 1)
			{
				printf("No joysticks connected.");
			}
			else
			{
				this->_joy = true;
			}

			//Create window
			gWindow = SDL_CreateWindow("Works?", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Create context
				gContext = SDL_GL_CreateContext(gWindow);
				if (gContext == NULL)
				{
					printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{
					glewExperimental = GL_TRUE;
					GLenum glewError = glewInit();
					if (glewError != GLEW_OK)
					{
						printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
					}

					//Use Vsync
					if (SDL_GL_SetSwapInterval(1) < 0)
					{
						printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
					}
					


				}
			}
		}
		if (TTF_Init() < 0)
		{
			printf("TTF could not initialize!");
			success = false;
		}

		

		return success;
	}

	bool get_joystick_status()
	{
		return this->_joy;
	}




    // Runs the engine.  Note that this technique takes no account of subsystem order.
    // If subsystem order is important consider using another mechanism.
    void run()
    {
		
/*
        // Initialise all the subsystems
        for (auto &sys : _self->_subsystems)
        {
            // If initialise fails exit run.
            if (!sys.second.initialise())
            {
                return;
            }
        }

        // Load content for all the subsystems
        for (auto &sys : _self->_subsystems)
        {
            // If load_content fails exit run.
            if (!sys.second.load_content())
            {
                return;
            }
        }
*/
		static unsigned int captureTypes[] =
		{
			SDL_KEYDOWN,
			SDL_KEYUP,
			SDL_MOUSEMOTION,
			SDL_MOUSEBUTTONDOWN,
			SDL_MOUSEBUTTONUP,
			SDL_MOUSEWHEEL,
			SDL_JOYAXISMOTION,
			SDL_JOYBALLMOTION,
			SDL_JOYHATMOTION,
			SDL_JOYBUTTONDOWN,
			SDL_JOYBUTTONUP,
			SDL_JOYDEVICEADDED,
			SDL_JOYDEVICEREMOVED,
			SDL_CONTROLLERAXISMOTION,
			SDL_CONTROLLERBUTTONDOWN,
			SDL_CONTROLLERBUTTONUP,
			SDL_CONTROLLERDEVICEADDED,
			SDL_CONTROLLERDEVICEREMOVED,
			SDL_FINGERDOWN,
			SDL_FINGERUP,
			SDL_FINGERMOTION,
		};

        // Loop until not running.
        while (_running)
        {
			bool moved = false;
			Uint8 hat_move = SDL_JoystickGetHat(this->get_subsystem<input_handler>()._self->gGameController_01, 0);
			Uint8 fire_button = SDL_JoystickGetButton(this->get_subsystem<input_handler>()._self->gGameController_01, 0);
			Uint8 start_button = SDL_JoystickGetButton(this->get_subsystem<input_handler>()._self->gGameController_01, 7);

			if (fire_button)
			{
				this->get_subsystem<Player_System>()._self->_data.at(0)->shooting = true;
			}

			if (start_button)
			{
				_running = false;
			}

			switch (hat_move)
			{
			case 0:
				//nothing
				break;
			case 1:
				// UP
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = -0.5;
				moved = true;
				break;
			case 2:
				// RIGHT
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = 0.5;
				moved = true;
				break;
			case 4:
				// DOWN
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = 0.5;
				moved = true;
				break;
			case 8:
				// LEFT
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = -0.5;
				moved = true;
				break;
			case 3:
				// UP-RIGHT
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = -0.5;
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = 0.5;
				moved = true;
				break;
			case 6:
				// RIGHT-DOWN
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = 0.5;
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = 0.5;
				moved = true;
				break;
			case 12:
				// DOWN-LEFT
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = 0.5;
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = -0.5;
				moved = true;
				break;
			case 9:
				// UP-LEFT
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = -0.5;
				this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = -0.5;
				moved = true;
				break;
			}
            //std::cout << "Engine Running" << std::endl;
			
			while (SDL_PollEvent(&e) != 0)
			{
				//std::cout << "still events" << std::endl;
				///// Check all input and compare - current version checks ALL input so no events are lost.
				///// Uncommented version compares only the input types we seek for checking if keys are pressed
			if(e.type == SDL_JOYDEVICEADDED)
			{
				if (this->get_subsystem<input_handler>()._self->JOYSTICK_ACCUM < this->get_subsystem<input_handler>()._self->JOYSTICK_INIT_COUNT)
				{
					this->get_subsystem<input_handler>()._self->JOYSTICK_ACCUM += 1;
					std::cout << "Continue Init Joystick... Count is " << this->get_subsystem<input_handler>()._self->JOYSTICK_ACCUM << std::endl;
					if (this->get_subsystem<input_handler>()._self->JOYSTICK_ACCUM == this->get_subsystem<input_handler>()._self->JOYSTICK_INIT_COUNT)
						this->get_subsystem<input_handler>()._self->JOYSTICK_INIT_FLAG = true;
					break;
				}
				else
				{
					std::cout << "Device added beyond init. Handling re-initialization." << std::endl;
					this->get_subsystem<input_handler>().InitializeJoysticks();
					break;
				}
			}
			// When a controller is removed
			if (e.type == SDL_JOYDEVICEREMOVED)
			{
				// Which player device was removed?

				// Player One
				// always Pause and offer for a controller reconnect
				// if reconnect not applicable or wanted, then select to continue game without player 1 controller

				// Player Two
				// Was Player 2 active?
				// YES - resolve, Pause and ask to re-input - or offer dropout of player two
				// NO - ignore - inconsequential

				// device removed
				std::cout << "Device removed." << std::endl;
				break;
			}

				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_w:
						//std::cout << "W" << std::endl;
						this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = -0.5;
						break;

					case SDLK_s:
						this->get_subsystem<Player_System>()._self->_data.at(0)->vel.z = 0.5;
						break;

					case SDLK_a:
						this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = -0.5;
						break;

					case SDLK_d:
						this->get_subsystem<Player_System>()._self->_data.at(0)->vel.x = 0.5;
						break;

					case SDLK_SPACE:
						this->get_subsystem<Player_System>()._self->_data.at(0)->shooting = true;
						break;
					}

					if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						for (int i = 0; i < 10; i++)
						{
							//std::cout << "WUITTING" << std::endl;
						}
						_running = false;
					}
				}
				//for (const auto &etype : captureTypes)
				//{
				//	if (e.type == etype)
				//	{
				//		//std::cout << "EVENT matched." << e.type << std::endl;
				//state_set = this->get_subsystem<input_handler>().HandleInputEvent(e);
				//		continue;
				//	}
				//}

				//if (e.type == SDL_JOYBUTTONDOWN)
				//{
				//	std::cout << (int)e.jbutton.button << std::endl;
				//}

				//User requests quit
				if (e.type == SDL_QUIT)
				{
					//std::cout << "Anything" << std::endl;
					_running = false;
				}

			}


			if (state_set != "")
			{
				// If the string was set to anything other than it's default "" value then the stae has been changed. see next slippet for more.
				//std::cout << state_set << std::endl;
				this->change_state(state_set, *this);
			}
			//else
			//{
			//	// If the return of the input handler's method hasn't changed the state_set string then the state has not changed -  WIP functionality as the state
			//	// will not be changed simply by button presses. there will be more logic to it than that. 
			//	//std::cout << "state unchanged" << std::endl;
			//}


			SDL_GetWindowSize(gWindow, &x, &y);
			glViewport(0, 0, x, y);
			state_set = new_state_set;

			if (state_set == "GAME")
			{
				//R// chuck in the volume changing stuff here
				//Not sure what the relevance of the & 0x8000 is but it doesn't work properly without it
				//Bumped from here https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
				if (GetKeyState('M') & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					Sounds.MuteMusic();
				}
				if (GetKeyState(VK_OEM_PLUS) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					Sounds.TurnUpMusic();
				}
				if (GetKeyState(VK_OEM_MINUS) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					Sounds.TurnDownMusic();
				}



				// Update the subsystems.  At the moment use dummy time of 1.0s.  You
				// will want to use a proper timer.
				for (auto &sys : _self->_subsystems)
				{
					sys.second.update(1.0);
				}

				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glClearColor(0.0, 1.0, 1.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Render the subsystems.
				for (auto &sys : _self->_subsystems)
				{
					sys.second.render();
				}




				SDL_GL_SetSwapInterval(1);

				SDL_GL_SwapWindow(gWindow);
			}
        }


		

        // Unload the content.
        for (auto &sys : _self->_subsystems)
        {
            sys.second.unload_content();
        }

        // Shutdown subsystems
        for (auto &sys : _self->_subsystems)
        {
            sys.second.shutdown();
        }
        // Clear out all the subsystems causing destructors to call.
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
        _self->_subsystems.clear();
		SDL_Quit();
        // Engine will now exit.
    }
};



