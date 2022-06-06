#ifndef __MODULE_PLAYER2_H__
#define __MODULE_PLAYER2_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer2 : public Module
{
public:
	// Constructor
	ModulePlayer2(bool startEnabled);

	// Destructor
	~ModulePlayer2();

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

	bool CleanUp() override;

public:
	// Position of the player in the map
	fPoint position;

	// The speed in which we move the player (pixels per frame)
	int speed;

	// Score Player 2
	int score2;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	SDL_Texture* dust_texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;
	Animation* dustAnimation = nullptr;

	// A set of animations
	Animation idleRAnim;
	Animation idleLAnim;
	Animation idleDisk;
	Animation upLAnim;
	Animation upRAnim;
	Animation downLAnim;
	Animation downRAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation lanzamiento;
	Animation polvo;
	Animation win;
	Animation lose;
	Animation dashUp;
	Animation dashDown;
	Animation dashLeft;
	Animation dashRight;
	Animation diagonalUpRight;
	Animation diagonalUpLeft;
	Animation diagonalDownRight;
	Animation diagonalDownLeft;
	Animation bloqueo;

	enum estadoPlayer2
	{
		STOP = 0,
		MOVIMIENTO,
		WITHFRISBEE,
		LANZAMIENTO,
		LANZAMIENTO_SUPER,
	};
	int estadoP2;

	enum estadoTimerP2 {
		INICIO,
		EJECUTANDO,
		FIN,
	};
	int estadoTP2;

	void timerP2();

	int initialTimeP2;
	int currentTimeP2;
	int timeLimitP2;

	//Last Move
	int last2;

	// The player's collider
	Collider* collider = nullptr;

	void movimientoPlayer2();
	void lanzamientoPlayer2();
	void limitePlayer2();


	// Font score index
	uint score;
	int scoreFont;
	char scoreText[10] = { "\0" };


	// Font round index
	int debugFont;
	int round;
	char debugText[10] = { "\0" };
	bool isDebugAppear;
	bool pols;

	int bea;
	float pepe;

	int posicionInicialX;
	int posicionInicialY;

	int ajusteColliderX;
	int ajusteColliderY;

	int lanzamientoXSpeed;
	int lanzamientoYSpeed;
	int parabolaXSpeed;
	int parabolaYSpeed;

	int ajusteColliderFrisbeeX;

	uint tossFX;
	uint hiromiChargeFX;
	uint hiromiPowerSoundFX;
	uint yooChargeFX;
	uint yooPowerSoundFX;
	uint wesselChargeFX;
	uint wesselPowerSoundFX;
	uint dashFX;
	bool stopLoadFX;
	bool startLoadingSupershot;

	bool bloqAnimation;
};

#endif //!__MODULE_PLAYER2_H__