#ifndef __SCENE_NEOGEO_H__
#define __SCENE_NEOGEO_H__
#define NUM_IMAGES 102

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneNeogeo : public Module
{
public:
	//Constructor
	SceneNeogeo(bool startEnabled);

	//Destructor
	~SceneNeogeo();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

	bool CleanUp() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture[NUM_IMAGES];

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	//Animation* currentAnimation = nullptr;

	int timer;
	int frame;
	int next = 0;
	bool scape = false;

	// Sound effects indices
	uint selectFx;

};

#endif
