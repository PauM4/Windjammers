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

	bool CleanUp() override;

public:
	// Position of the player in the map
	fPoint position;

	// The speed in which we move the player (pixels per frame)
	float speed;
	float xspeed;
	float yspeed;
	int mov;
	int arbitro;
	int FloorTime;

	int contadorBlock;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation2 = nullptr;

	// A set of animations
	Animation moving;
	Animation projectile;
	Animation blocked;
	Animation desaparece;
	Animation stop;

	// The player's collider
	Collider* collider = nullptr;
	Collider* provisional = nullptr;
	Collider* provisional2 = nullptr;

	

	enum estadoFrisbee
	{
		ARBITROF,
		STOP, //en el suelo
		MOVIMIENTO, //en el aire
		PRUEBA,
		WITHPLAYER, //con el player + cambiar posicion
		BLOCK,
		SUELO,
		LANZAMIENTOARBITRO

	};
	int estadoF;

	enum tipoLanzamiento {
		NORMAL,
		PARABOLA,
		ARBITRO,
		SUPERSHOT, 
		BLOCKPLAYER1,
		BLOCKPLAYER2
	};
	int lanzamientoF;

	enum tipoSupershot {
		MITA_SUPERSHOT,
		YOO_SUPERSHOT,
		WESSEL_SUPERSHOT,
		NONE
	};
	int tipoSupershot;
	
	enum direccionFrisbeePlayer {
		DARRIBA,
		HORIZONTAL,
		DABAJO,
		MAX

	};
	int direccionF;

	void movimientoFrisbee();
	void limitesFrisbee();

	enum estadoTimerF {
		INICIO,
		EJECUTANDO,
		FIN,
	};
	int estadoTF;

	void timerF();
	int initialTimeF;
	int currentTimeF;
	int timeLimitF;

	void vel_parabola(int pos_Player, int pos_final_frisbee);

	bool blockSuperShot;

	int limiteWesselSupershot;

	void anguloSupershot();
	float angulo;
	int contadorYooSuperShot;
	bool yooDirec;
	bool contador_Angulo_SuperShot; //contador utilizado tanto para el angulo del supershot de la Mito como para el supershot de Yoo
	
	bool limiteConcrete;

	int parabolaFinalX;
	int parabolaFinalY;
	bool indicacionPlayerParabola;

	uint catchFx;
	uint effectTossFx; //LANZAMIENTOTF = PARABOLA posar abans del canvi
	uint frisbeeOnAirFX; //
	uint landingFX;
	uint wallHitFX;

	uint hiromiSuperSonicFX;
	uint hiromiSuperFFX;

	uint yooSuperSonicFX;
	uint yooSuperFFX;

	uint wesselSuperSonicFX;
	uint wesselSuperFFX;

};

#endif //!__MODULE_FRISBEE_H__ 
