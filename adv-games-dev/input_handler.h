#pragma once

#include <iostream>
#include <memory>
#include "singleton.h"
#include "entity.h"
#include <string>
#include "../include/SDL.h"

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 5000;

// This class is purely code so we don't worry about
// PIMPL here.
class input_handler : public singleton<input_handler>
{
	friend class singleton<input_handler>;
private:
	struct joystick_data_impl
	{
		SDL_Joystick* gGameController_01;
		SDL_Joystick* gGameController_02;

		SDL_Haptic* gControllerHaptic_01;
		SDL_Haptic* gControllerHaptic_02;

		SDL_JoystickID PLAYER_ONE_JOYSTICK;
		SDL_JoystickID PLAYER_TWO_JOYSTICK;

		std::unordered_map<int, SDL_Joystick*> controllers;
		std::unordered_map<int, SDL_Haptic*> haptics;

		bool JOYSTICK_INIT_FLAG;;
		int JOYSTICK_ACCUM;
		int JOYSTICK_INIT_COUNT;
	};

	input_handler() : _self{ new joystick_data_impl() }
	{
		_self->gGameController_01 = NULL;
		_self->gGameController_02 = NULL;

		_self->gControllerHaptic_01 = NULL;
		_self->gControllerHaptic_02 = NULL;

		_self->PLAYER_ONE_JOYSTICK = NULL;
		_self->PLAYER_TWO_JOYSTICK = NULL;

		_self->JOYSTICK_INIT_FLAG = false;
		_self->JOYSTICK_ACCUM = 0;
		_self->JOYSTICK_INIT_COUNT = 0;

		std::cout << "Input handler initialised. Controller and Haptics loaded and set." << std::endl;
	}


public:

	std::shared_ptr<joystick_data_impl> _self = nullptr;

	bool InitializeJoysticks()
	{
		// Wipe all values
		_self->gGameController_01 = NULL;
		_self->gGameController_02 = NULL;

		_self->gControllerHaptic_01 = NULL;
		_self->gControllerHaptic_02 = NULL;

		_self->PLAYER_ONE_JOYSTICK = NULL;
		_self->PLAYER_TWO_JOYSTICK = NULL;

		// Report controller count
		std::cout << SDL_NumJoysticks() << " controller(s) are currently connected.\n" << std::endl;

		if (!_self->JOYSTICK_INIT_FLAG)
		{
			_self->JOYSTICK_INIT_COUNT = SDL_NumJoysticks();
		}

		if (SDL_NumJoysticks() <= 0)
		{
			std::cout << "No controllers currently connected. Switch to Keyboard Layout." << std::endl;
			return 1;
		}
		else
		{
			if (SDL_NumJoysticks() >= 1)
			{
				// Initialize Player One Joystick On Startup
#pragma region Player One Controller Init()

				// Open SDL Joystick on the first GameController
				_self->gGameController_01 = SDL_JoystickOpen(0);

				// If NULL, unable to open joystick (incompatible controller?)
				if (_self->gGameController_01 == NULL)
				{
					printf("Unable to open game controller object!");
				}

				// Assign the SystemWide Player One ID
				_self->PLAYER_ONE_JOYSTICK = SDL_JoystickInstanceID(_self->gGameController_01);

				// Assign this Instance of the current Player One controller to the controller map
				_self->controllers.emplace(SDL_JoystickInstanceID(_self->gGameController_01), _self->gGameController_01);

				// Success so far, assign to controller map
				if (SDL_IsGameController(0)) std::cout << "DEBUG: Index is true, controller is initialized." << std::endl;

				// Report Player One Controller Information
				std::cout << "Controller 1 InstanceID is set to " << SDL_JoystickInstanceID(_self->gGameController_01) << std::endl;
				std::cout << "Number of axes on Controller 1 is: " << SDL_JoystickNumAxes(_self->gGameController_01) << std::endl;
				std::cout << "Button Count = " << SDL_JoystickNumButtons(_self->gGameController_01) << std::endl;

				// Initialize Controller One rumble feature, if available.
				_self->gControllerHaptic_01 = SDL_HapticOpenFromJoystick(_self->gGameController_01);
				if (_self->gControllerHaptic_01 == NULL)
				{
					std::cout << "Controller 1 does not support Haptic." << std::endl;
				}
				else
				{
					if (SDL_HapticRumbleInit(_self->gControllerHaptic_01) < 0)
					{
						std::cout << "Unable to init() Rumble on Controller 1." << std::endl;
					}
					else
					{
						if (SDL_HapticRumblePlay(_self->gControllerHaptic_01, 0.75, 100) != 0)
						{
							std::cout << "Error on Rumble Init for Controller 1." << std::endl;
						}
						// Success so far, insert haptic into the map at index of first controller InstanceID
						_self->haptics.emplace(SDL_JoystickInstanceID(_self->gGameController_01), _self->gControllerHaptic_01);
					}
				}
#pragma endregion
			}

			if (SDL_NumJoysticks() >= 2)
			{
				// IF applicable, Initialize Player Two Joystick On Startup
#pragma region Player Two Controller Init()

				// Open SDL Joystick on the first GameController
				_self->gGameController_02 = SDL_JoystickOpen(1);

				// If NULL, unable to open joystick (incompatible controller?)
				if (_self->gGameController_02 == NULL)
				{
					printf("Unable to open game controller object!");
				}

				// Assign the SystemWide Player One ID
				_self->PLAYER_TWO_JOYSTICK = SDL_JoystickInstanceID(_self->gGameController_02);

				// Assign this Instance of the current Player One controller to the controller map
				_self->controllers.emplace(SDL_JoystickInstanceID(_self->gGameController_02), _self->gGameController_02);

				// Success so far, assign to controller map
				if (SDL_IsGameController(1)) std::cout << "DEBUG: Index is true, controller is initialized." << std::endl;

				// Report Player One Controller Information
				std::cout << "Controller 2 InstanceID is set to " << SDL_JoystickInstanceID(_self->gGameController_01) << std::endl;
				std::cout << "Number of axes on Controller 2 is: " << SDL_JoystickNumAxes(_self->gGameController_01) << std::endl;
				std::cout << "Button count = " << SDL_JoystickNumButtons(_self->gGameController_01) << std::endl;

				// Initialize Controller One rumble feature, if available.
				_self->gControllerHaptic_02 = SDL_HapticOpenFromJoystick(_self->gGameController_02);
				if (_self->gControllerHaptic_02 == NULL)
				{
					std::cout << "Controller 2 does not support Haptic." << std::endl;
				}
				else
				{
					if (SDL_HapticRumbleInit(_self->gControllerHaptic_02) < 0)
					{
						std::cout << "Unable to init() Rumble on Controller 2." << std::endl;
					}
					else
					{
						if (SDL_HapticRumblePlay(_self->gControllerHaptic_02, 0.75, 1500) != 0)
						{
							std::cout << "Error on Rumble Init for Controller 2." << std::endl;
						}
						// Success so far, insert haptic into the map at index of first controller InstanceID
						_self->haptics.emplace(SDL_JoystickInstanceID(_self->gGameController_02), _self->gControllerHaptic_02);
					}
				}
#pragma endregion
			}
		}

		return 0;
	}

	bool initialise()
	{
		return true;
	}

	void update(float delta_time)
	{
		// Should never be called
		//std::cout << "Renderer updating" << std::endl;
	}

	bool load_content()
	{
		//std::cout << "Input handler loading content" << std::endl;
		return true;
	}

	std::string HandleInputEvent(SDL_Event &e)
	{
		// Init state container - WIP change for game logic later
		std::string state = "";
		//std::cout << "Input handler updating on button input..." << std::endl;
		switch (e.type)
		{

		case SDL_JOYDEVICEADDED:
		{
			if (_self->JOYSTICK_ACCUM < _self->JOYSTICK_INIT_COUNT)
			{
				_self->JOYSTICK_ACCUM += 1;
				std::cout << "Continue Init Joystick... Count is " << _self->JOYSTICK_ACCUM << std::endl;
				if (_self->JOYSTICK_ACCUM == _self->JOYSTICK_INIT_COUNT)
					_self->JOYSTICK_INIT_FLAG = true;
				break;
			}
			else
			{
				std::cout << "Device added beyond init. Handling re-initialization." << std::endl;
				InitializeJoysticks();
				break;
			}
		}
		// When a controller is removed
		case SDL_JOYDEVICEREMOVED:
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
		//User requests quit
		case SDL_QUIT:
		{
			std::cout << "Application quit. NOT IMPLEMENTED (need a reference to game state to quit)." << std::endl;
			break;
		}
		// INPUT CODE HERE
		case SDL_JOYAXISMOTION:
		{
			if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
			{
				std::cout << "axis" << e.jaxis.which << " : value " << e.jaxis.value << std::endl;
			}
			break;
		}

		case SDL_CONTROLLERAXISMOTION:
		{
			std::cout << "rotating. - CONTROLLER" << std::endl;
			break;
		}

		case SDL_KEYDOWN:
		{
			std::cout << "Keypress" << std::endl;
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				state = "EXIT";
				return state;
			}
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
			{
				std::cout << "Application quit. NOT IMPLEMENTED (need a reference to game state to quit)." << std::endl;
				//quit = true;
				state = "EXIT";
				return state;
			}

			case SDL_SCANCODE_0:
			{
				SDL_HapticRumblePlay(_self->haptics.at(_self->PLAYER_ONE_JOYSTICK), 0.75, 500);
				break;
			}

			case SDL_SCANCODE_1:
			{
				SDL_HapticRumblePlay(_self->haptics.at(_self->PLAYER_TWO_JOYSTICK), 0.75, 500);
				break;
			}
			}
		}

		case SDL_JOYBUTTONDOWN:
		{
			// TODO switch to case type
#pragma region Player 1
			if (e.jbutton.which == _self->PLAYER_ONE_JOYSTICK)
			{

				if ((unsigned)e.jbutton.button == 0)
				{
					std::cout << " PLAYER ONE: " << "FIRE BUTTON" << std::endl;
					state = "MAIN";
					break;
				}
				else if ((unsigned)e.jbutton.button == 1)
				{
					std::cout << " PLAYER ONE: " << "BOMB BUTTON" << std::endl;
					state = "GAME";
					break;
				}
				else if ((unsigned)e.jbutton.button == 2)
				{
					std::cout << " PLAYER ONE: " << "DOGE(JUMP) BUTTON" << std::endl;
					state = "PAUSED";
					break;
				}
				else if ((unsigned)e.jbutton.button == 3)
				{
					std::cout << " PLAYER ONE: " << "MENU BUTTON" << std::endl;
					break;
				}
				else if ((unsigned)e.jbutton.button == 4)
				{
					std::cout << " PLAYER ONE: " << "CHANGE WEAPON (LEFT) BUTTON" << std::endl;
					break;
				}
				else if ((unsigned)e.jbutton.button == 5)
				{
					std::cout << " PLAYER ONE: " << "CHANGE WEAPON (RIGHT) BUTTON" << std::endl;
					break;
				}
				else if ((unsigned)e.jbutton.button == 6)
				{
					std::cout << " PLAYER ONE: " << "SELECT BUTTON" << std::endl;
					break;
				}
				else if ((unsigned)e.jbutton.button == 7)
				{
					std::cout << " PLAYER ONE: " << "START BUTTON" << std::endl;
					state = "EXIT";
					break;
				}
			}
#pragma endregion

#pragma region Player 2
			else if (e.jbutton.which == _self->PLAYER_TWO_JOYSTICK)
			{
				std::cout << " NO PLAYER 2 CONTROLLER INPUT CURRENTLY!" << std::endl;
				if ((unsigned)e.jbutton.button == 0)
				{
					std::cout << " PLAYER TWO: " << "FIRE BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 1)
				{
					std::cout << " PLAYER TWO: " << "BOMB BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 2)
				{
					std::cout << " PLAYER TWO: " << "DOGE(JUMP) BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 3)
				{
					std::cout << " PLAYER TWO: " << "MENU BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 4)
				{
					std::cout << " PLAYER TWO: " << "CHANGE WEAPON (LEFT) BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 5)
				{
					std::cout << " PLAYER TWO: " << "CHANGE WEAPON (RIGHT) BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 6)
				{
					std::cout << " PLAYER TWO: " << "SELECT BUTTON" << std::endl;
				}
				else if ((unsigned)e.jbutton.button == 7)
				{
					std::cout << " PLAYER TWO: " << "START BUTTON" << std::endl;
				}
			}
#pragma endregion
		}
		}
		return state;
	}

	void render()
	{
		// This should never be called.
		//std::cout << "Input handler rendering" << std::endl;
	}

	void unload_content()
	{
		//std::cout << "Input handler unloading content" << std::endl;
	}

	void shutdown()
	{
		//std::cout << "Input handler shutting down" << std::endl;
	}
};
