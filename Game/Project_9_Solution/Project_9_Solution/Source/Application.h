#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 19

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModulePlayer;
class ModulePlayer2;
class ModuleFrisbee;
class SceneIntro;
class SceneTitle;
class SceneCharacterSelect;
class SceneStageSelect;
class SceneCharacterPresent;
class SceneBeachStage;
class ModuleParticles;
class ModuleCollisions;
class ModuleEnemies;
class ModuleFadeToBlack;
class ModuleFonts;
class ModuleRender;

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();

	//Destructor. Removes all module objects
	~Application();

	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	Update_Status Update();

	//Releases all the application data
	bool CleanUp();

public:
	// An array to store all modules
	Module* modules[NUM_MODULES];

	// All the modules stored individually
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;

	ModulePlayer* player = nullptr;
	ModulePlayer2* player2 = nullptr;
	ModuleFrisbee* frisbee = nullptr;

	SceneIntro* sceneIntro = nullptr;
	SceneTitle* sceneTitle = nullptr;
	SceneCharacterSelect* sceneCharacterSelect = nullptr;
	SceneStageSelect* sceneStageSelect = nullptr;
	SceneCharacterPresent* sceneCharacterPresent = nullptr;
	SceneBeachStage* sceneBeachStage = nullptr;

	ModuleEnemies* enemies = nullptr;
	ModuleParticles* particles = nullptr;

	ModuleCollisions* collisions = nullptr;
	ModuleFadeToBlack* fade = nullptr;
	ModuleFonts* fonts = nullptr;

	ModuleRender* render = nullptr;
};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__