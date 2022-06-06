#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput(bool startEnabled) : Module(startEnabled)
{}

ModuleInput::~ModuleInput()
{}

bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);
	
	SDL_GameControllerAddMapping("030000006f0e00008401000000000000, ,platform:Windows,a:b1,b:b2,x:b0,y:b3,back:b8,guide:b12,start:b9,leftstick:b10,rightstick:b11,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7,");

	SDL_Init(SDL_INIT_EVERYTHING);

	for (int i = 0; i < SDL_NumJoysticks(); i++) {

		if (SDL_IsGameController(i) && sdl_P1 == nullptr)
		{
			sdl_P1 = SDL_GameControllerOpen(i);
			LOG("Controller 1 was found");
			break;
		}
		else if (SDL_IsGameController(i) && sdl_P1 != nullptr && sdl_P2 == nullptr)
		{
			sdl_P2 = SDL_GameControllerOpen(i);
			LOG("Controller 2 was found");
			break;
		}
	}
	if (sdl_P1 == nullptr && sdl_P2 == nullptr)
	{
		LOG("No Controllers found")
	}

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

Update_Status ModuleInput::PreUpdate()
{
	//Read new SDL events, mostly from the window
	//SDL_Event event;
	//if (SDL_PollEvent(&event))
	//{
	//	if (event.type == SDL_QUIT)	return Update_Status::UPDATE_STOP;
	//}

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_REPEAT /*|| App->input->pads->l1*/)	return Update_Status::UPDATE_STOP;

	}

	//Read all keyboard data and update our custom array
	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	SDL_GameControllerUpdate();

	for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j)
	{
		if (SDL_GameControllerGetButton(sdl_P1, (SDL_GameControllerButton)j))
			P1.buttons[j] = (P1.buttons[j] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			P1.buttons[j] = (P1.buttons[j] == KEY_REPEAT || P1.buttons[j] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	if (P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN] != KEY_IDLE)	keys[SDL_SCANCODE_S] = P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_UP] != KEY_IDLE)		keys[SDL_SCANCODE_W] = P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_UP];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT] != KEY_IDLE)	keys[SDL_SCANCODE_A] = P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] != KEY_IDLE)	keys[SDL_SCANCODE_D] = P1.buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_A] != KEY_IDLE)			keys[SDL_SCANCODE_V] = P1.buttons[SDL_CONTROLLER_BUTTON_A];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_B] != KEY_IDLE)			keys[SDL_SCANCODE_B] = P1.buttons[SDL_CONTROLLER_BUTTON_B];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_Y] != KEY_IDLE)			keys[SDL_SCANCODE_N] = P1.buttons[SDL_CONTROLLER_BUTTON_Y];
	if (P1.buttons[SDL_CONTROLLER_BUTTON_START] != KEY_IDLE)		keys[SDL_SCANCODE_SPACE] = P1.buttons[SDL_CONTROLLER_BUTTON_START];

	for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j)
	{
		if (SDL_GameControllerGetButton(sdl_P2, (SDL_GameControllerButton)j))
			P2.buttons[j] = (P2.buttons[j] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			P2.buttons[j] = (P2.buttons[j] == KEY_REPEAT || P2.buttons[j] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	if (P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN] != KEY_IDLE)	keys[SDL_SCANCODE_DOWN] = P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_UP] != KEY_IDLE)	keys[SDL_SCANCODE_UP] = P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_UP];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT] != KEY_IDLE)	keys[SDL_SCANCODE_LEFT] = P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] != KEY_IDLE)	keys[SDL_SCANCODE_RIGHT] = P2.buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_A] != KEY_IDLE)	keys[SDL_SCANCODE_O] = P2.buttons[SDL_CONTROLLER_BUTTON_A];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_B] != KEY_IDLE)	keys[SDL_SCANCODE_P] = P2.buttons[SDL_CONTROLLER_BUTTON_B];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_Y] != KEY_IDLE)	keys[SDL_SCANCODE_I] = P2.buttons[SDL_CONTROLLER_BUTTON_Y];
	if (P2.buttons[SDL_CONTROLLER_BUTTON_START] != KEY_IDLE)		keys[SDL_SCANCODE_RETURN] = P2.buttons[SDL_CONTROLLER_BUTTON_START];


	

	return Update_Status::UPDATE_CONTINUE;
}




bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
