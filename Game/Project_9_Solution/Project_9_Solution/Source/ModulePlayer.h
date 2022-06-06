#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	// Constructor
	ModulePlayer(bool startEnabled);

	// Destructor
	~ModulePlayer();

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

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;



	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	//Animation* dustAnimation = nullptr;

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

	enum estadoPlayer
	{
		STOP =0,
		MOVIMIENTO,
		WITHFRISBEE,
		LANZAMIENTO,
		LANZAMIENTO_SUPER,
		
	};
	int estadoP1;

	enum estadoTimerP {
		INICIO,
		EJECUTANDO,
		FIN,
	};
	int estadoTP;

	void timerP();
	int initialTimeP;
	int currentTimeP;
	int timeLimitP;


	//Last Move
	int last1;

	// The player's collider
	Collider* collider = nullptr;
	
	void movimientoPlayer();
	void lanzamientoPlayer();
	void limitePlayer();
	

	// Font score index
	uint score;
	int scoreFont;
	char scoreText[10] = { "\0" };

	// Font round index
	int debugFont;
	char debugText[10] = { "\0" };
	bool isDebugAppear;

	//Numero de rondas ganadas
	int round;
	int bea;
	float pepe;
	
	//bool pols;

	//magicNumbers
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

#endif //!__MODULE_PLAYER_H__