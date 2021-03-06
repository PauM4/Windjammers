#pragma once
#ifndef __SCENE_STAGE_SELECT_H__
#define __SCENE_STAGE_SELECT_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

//Enum with all maps
//Quan tinguem altres mapes, es fara servir
enum MapList
{
	Beach,
	Lawn,
	Tiled,
	Concrete,
	Clay,
	Stadium
};

class SceneStageSelect : public Module
{
public:
	//Constructor
	SceneStageSelect(bool startEnabled);

	//Destructor
	~SceneStageSelect();

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
	SDL_Texture* bgStageTexture = nullptr;
	SDL_Texture* stageNamesTexture = nullptr;
	SDL_Texture* selectSquareTexture = nullptr;

	SDL_Texture* miniClayTexture = nullptr;
	SDL_Texture* miniConcreteTexture = nullptr;
	SDL_Texture* miniLawnTexture = nullptr;
	SDL_Texture* miniStadiumTexture = nullptr;
	SDL_Texture* miniTiledTexture = nullptr;
	SDL_Texture* miniBeachTexture = nullptr;

	SDL_Rect selectSquare;

	//Scene selected
	int sceneSelected;

	// Sound effects indices
	uint selectFx;
	uint moveFx;

	//Coordinates
	int yMove, x1, x2, x3;

	//Font Debug vars
	int debugFont;
	char debugText[10] = { "\0" };
	bool isDebugAppear;
};

#endif

