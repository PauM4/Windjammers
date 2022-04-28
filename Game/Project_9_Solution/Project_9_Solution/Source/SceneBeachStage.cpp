#include "SceneBeachStage.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModuleFrisbee.h"

#include "SDL/include/SDL.h"


SceneBeachStage::SceneBeachStage(bool startEnabled) : Module(startEnabled)
{
	//Load animation Timer test
	int posX = 15;
	for (int i = 0; i < 32; ++i)
	{
		timerAnim.PushBack({ posX, 0, 15, 15 });
		posX += 15;
	}
	timerAnim.loop = false;
	timerAnim.pingpong = false;
	timerAnim.speed = 0.017f;

	//Load beach bg animation
	bgBeachAnim.PushBack({ 0, 0, 304, 224 });
	bgBeachAnim.PushBack({ 304, 0, 304, 224 });
	bgBeachAnim.PushBack({ 608, 0, 304, 224 });
	bgBeachAnim.PushBack({ 0, 224, 304, 224 });
	//bgBeachAnim.PushBack({ 304, 224, 304, 224 });
	bgBeachAnim.speed = 0.2f;

}

SceneBeachStage::~SceneBeachStage()
{

}

// Load assets
bool SceneBeachStage::Start()
{
	
	

	LOG("Loading background assets");

	bool ret = true;

	suddenDeath = false;
	initialTime = 0;
	startTheGame = false;

	//FX de rounds
	round1FX = App->audio->LoadFx("Assets/Fx/round1.wav");


	//beachTexture = App->textures->Load("Assets/Sprites/Levels/PH_Beach.png");
	bgBeachTexture = App->textures->Load("Assets/Sprites/Levels/bgBeachSpriteSheet.png");
	currentBeachAnim = &bgBeachAnim;

	App->audio->PlayMusic("Assets/Music/03_Flying Power Disc (Beach Court).ogg", 1.0f);
	
	// Load timer texture
	timerTexture = App->textures->Load("Assets/Sprites/UI/Fonts/timerSpriteSheet.png");
	currentTimerAnim = &timerAnim;

	// Load UI Texture
	uiSpriteTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->player2->Enable();
	App->frisbee->Enable();
	App->collisions->Enable();

	return ret;
}

Update_Status SceneBeachStage::Update()
{
	//240 == 4s
	if (initialTime < 240)
	{
		initialTime++;
	}
	else if (initialTime == 240)
	{
		startTheGame = true;
		//EndRound(1);
	}

	if (startTheGame)
	{
		if (time <= 1860)
		{
			time++;
		}
		currentTimerAnim->Update();
	}

	currentBeachAnim->Update();

	// DEBUG INSTANT WIN
	if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN)
	{
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		SDL_Delay(1500);
		App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}
	// DEBUG INSTANT LOSE
	if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN)
	{
		App->audio->PlayMusic("Assets/Music/09_Lost Set.ogg", 0.0f);
		SDL_Delay(500);
		App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}

	//ERROR Se ejecuta el audio todo el rato, debe de haber una manera de que se ejecute con una tecla para pasar de ronda
	//App->audio->PlayFx(round1FX);

	ScoreRound(0);

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneBeachStage::PostUpdate()
{

	// Draw everything --------------------------------------
	//App->render->Blit(beachTexture, 0, 0, NULL);
	//Beach background
	SDL_Rect rectBeach = currentBeachAnim->GetCurrentFrame();
	App->render->Blit(bgBeachTexture, 0, 0, &rectBeach);

	if (startTheGame)
	{
		//Timer
		SDL_Rect rectTimer = currentTimerAnim->GetCurrentFrame();
		App->render->Blit(timerTexture, 144, 13, &rectTimer);
	}
	else
	{
		SDL_Rect rectNormes = { 207, 11, 151, 15 };
		App->render->Blit(uiSpriteTexture, 75, 150, &rectNormes);
	}

	//P1 UI
	SDL_Rect p1Rect = { 359, 0, 15, 8 };
	App->render->Blit(uiSpriteTexture, 10, 10, &p1Rect);

	//P2 UI
	SDL_Rect p2Rect = { 374, 0, 16, 8 };
	App->render->Blit(uiSpriteTexture, 200, 10, &p2Rect);

	//Japan Flag x2
	SDL_Rect japanFlagRect = { 460, 0, 15, 9 };
	App->render->Blit(uiSpriteTexture, 40, 10, &japanFlagRect);
	App->render->Blit(uiSpriteTexture, 230, 10, &japanFlagRect);

	//Timer
	SDL_Rect rectTimer = currentTimerAnim->GetCurrentFrame();
	App->render->Blit(timerTexture, 144, 13, &rectTimer);

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneBeachStage::CleanUp()
{
	App->player->Disable();
	App->player2->Disable();
	App->frisbee->Disable();
	App->collisions->Disable();

	return true;
}

//En cuanto mete un jugador un gol, se llama a esto y se determina el valor de arbitro. Lo ponemos aqui como funcion externa en vez de dentro del update de frisbee
//Ya que en este .cpp también llamaremos a esta función en función de las rondas/sets ganados~
void SceneBeachStage::EndRound(int arbitro) { 
	if (arbitro == 1) {
		App->frisbee->arbitro = 1;
	}
	if (arbitro == 2) {
		App->frisbee->arbitro = 2;
	}
}

void SceneBeachStage::ScoreRound(int arbitro) {

	//Cambiar de Ronda y Dar puntos de la ronda
	if (App->player->score != App->player2->score) { //|| App->player->score > 12 || App->player2->score > 12

		if (App->player->score >= 12 || App->player2->score >= 12) {

			if (App->player->score > App->player2->score + 2) {
				App->player->round += 1;
				
				Win();
				//Llamar animación de jugador ganador 1 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				EndRound(2);
				
				timerAnim.Reset();
			}

			if (App->player2->score > App->player->score + 2) {
				App->player2->round += 1;
				
				Win();
				//Llamar animación de jugador ganador 2 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				EndRound(1);
				timerAnim.Reset();
			}

		} 
		else if (time == 1860) {
			
			 if (App->player->score > App->player2->score) {
				App->player->round += 1;
				
				Win();
				//Llamar animación de jugador ganador 1 y las texturas
				App->player->score = 0; 
				App->player2->score = 0;
				EndRound(2);
				timerAnim.Reset();
				
				time = 0;
			} 
			else if (App->player2->score > App->player->score) {
				App->player2->round += 1;
				
				Win();
				//Llamar animación de jugador ganador 2 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				EndRound(1);
				timerAnim.Reset();
				
				time = 0;
			}
		}
		else { //cuando la puntucion es diferente pero no se da nada de arriba, llamamos igualmente a la funcion que ahce que el arbitro manda de nuevo la bola 
			EndRound(arbitro);
		}

	}
	else if (App->player->score == App->player2->score && time >=1860) {
		App->player->round += 1;
		App->player2->round += 1;
		App->player->score = 0;
		App->player2->score = 0;
		
		Win();
		//Animación de cuando los dos acaban una ronda en puntuacion empate
		timerAnim.Reset();
		EndRound(1);
		time = 0;
	}
	else  { //puntuaciones empates
		EndRound(arbitro);
	}
}

void SceneBeachStage::Win() {

	if (App->player->round == App->player2->round && App->player->round == 2 && App->player2->round ==2 && !suddenDeath) {
		suddenDeath = true;
		EndRound(1);
	} 
	
	else if (App->player->score != 0 && suddenDeath) {
		//llamar animación y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}
	
	else if (App->player2->score != 0 && suddenDeath) {
		//llamar animación y texturas de que ha ganado el segundo jugador la partida
		//SDL Delay
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}

	else if (App->player->round == 2 && !suddenDeath) {
		//llamar animación y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	
	}

	else if (App->player2->round == 2&&!suddenDeath) {
		//llamar animación y texturas de que ha ganado el segundo jugador la partida
			//SDL Delay
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);

	}

	else if (suddenDeath && App->player->score == App->player2->score) {
		App->player->round = 69;
		//Animacion y texturas de que los dos han perdido
		//SDL Delay
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}
}
