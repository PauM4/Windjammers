#ifndef __SCENE_CHARACTER_SELECT_H__
#define __SCENE_CHARACTER_SELECT_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

//Quan tinguem més personatges, es fara servir
enum CharList
{
	Mita,
	Yoo,
	Costa,
	Biaggi,
	Scott,
	Wessel
};

class SceneCharacterSelect : public Module
{
public:
	//Constructor
	SceneCharacterSelect(bool startEnabled);

	//Destructor
	~SceneCharacterSelect();

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
	SDL_Texture* uiSpriteSheet = nullptr;

	// 1p, 2p sprite positions

	// Rects de 1p, 2p
	SDL_Rect p1Rect;
	SDL_Rect p2Rect;


	// Sound effects indices
	uint moveFx;
	uint hiromiSelected;

	///Coordinates
	int x1, y1;
	int x2, y2;


};

#endif