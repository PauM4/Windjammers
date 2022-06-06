#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL_gamecontroller.h"

#define MAX_KEYS 256
#define MAX_PADS 4
#define MAX_CONTROLLERS 12

struct _SDL_GameController;
struct _SDL_Haptic;

enum Key_State
{
	KEY_IDLE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct GameController {
	int a, b, y, x, dpad_up, dpad_down, dpad_left, dpad_right, start;
};

class ModuleInput : public Module
{
public:
	// Constructor
	ModuleInput(bool startEnabled);

	// Destructor
	~ModuleInput();

	// Called on application start.
	// Initializes the SDL system for input detection
	bool Init() override;

	// Called at the beginning of the application loop
	// Updates all input data received from SDL
	Update_Status PreUpdate() override;

	// Called on application exit.
	// Uninitializes the SDL system for input detection
	bool CleanUp() override;

public:
	// An array to fill in the state of all the keyboard keys
	Key_State keys[MAX_KEYS] = { KEY_IDLE };

	struct controller {
		Key_State buttons[SDL_CONTROLLER_BUTTON_MAX];
	};

	SDL_GameController* sdl_P1 = nullptr; 
	SDL_GameController* sdl_P2 = nullptr; 
	controller P1;
	controller P2;

	//SDL_GameController* sdl_controllers[MAX_CONTROLLERS];
	//SDL_GameController* sdl_controllers;
	//SDL_GameControllerButton* sdl_controllers[MAX_CONTROLLERS];
	
	//GameController controllers[MAX_CONTROLLERS];
	//GameController controllers;
	//int num_controllers;
};

#endif // __ModuleInput_H__