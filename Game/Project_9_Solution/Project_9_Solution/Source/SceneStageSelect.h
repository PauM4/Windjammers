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

	// Sound effects indices
	uint selectFx = 0;
	uint moveFx = 0;

	//Coordinates
	int yMove, x1, x2, x3, x4, x5, x6;
};

#endif