#ifndef __MODULE_FRISBEE_H__
#define __MODULE_FRISBEE_H__ 

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModuleFrisbee : public Module
{
public:
	// Constructor
	ModuleFrisbee(bool startEnabled);

	// Destructor
	~ModuleFrisbee();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	Update_Status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	Update_Status PostUpdate() override;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2) override;

public:
	// Position of the player in the map
	iPoint position;

	// The speed in which we move the player (pixels per frame)
	int speed = 1;
	int xspeed;
	int yspeed;
	int mov;
	int arbitro = 1;

	bool pared = false;
	bool inicio = false;
	bool posesion = false; //FALSE: PLAYER 1, TRUE: PLAYER 2

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation2 = nullptr;

	// A set of animations
	Animation moving;
	Animation projectile;


	// The player's collider
	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	void movimiento(int mov);

};

#endif //!__MODULE_FRISBEE_H__ 