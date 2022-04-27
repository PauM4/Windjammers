#pragma once
#ifndef __SCENE_CHAR_PRESENT_H__
#define __SCENE_CHAR_PRESENT_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneCharacterPresent : public Module
{
public:
	//Constructor
	SceneCharacterPresent(bool startEnabled);

	//Destructor
	~SceneCharacterPresent();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* charactersPresentTexture = nullptr;

	Animation* currentAnimation = nullptr;

	// Sound effects indices
	//uint hiromiSelected = 0;

	// Background animation
	Animation bgEffectAnim;

	int timer;
	bool scape;
};

#endif