#include "SceneBeachStage.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModuleFrisbee.h"
#include "ModuleFonts.h"
#include "SceneCharacterPresent.h"
#include "SceneStageSelect.h"
#include "ModuleIngameUI.h"
#include "SceneCharacterSelect.h"
#include "SceneTitle.h"


#include "SDL/include/SDL.h"

#include <stdio.h>
#include <conio.h>


SceneBeachStage::SceneBeachStage(bool startEnabled) : Module(startEnabled)
{
	//Load beach bg animation
	bgBeachAnim.PushBack({ 0, 0, 304, 224 });
	bgBeachAnim.PushBack({ 0, 224, 304, 224 });
	bgBeachAnim.PushBack({ 0, 448, 304, 224 });
	bgBeachAnim.PushBack({ 0, 672, 304, 224 });
	bgBeachAnim.speed = 0.2f;

	//Load lawn bg animation
	bgLawnAnim.PushBack({ 0, 0, 304, 224 });
	bgLawnAnim.PushBack({ 0, 224, 304, 224 });
	bgLawnAnim.PushBack({ 0, 448, 304, 224 });
	bgLawnAnim.PushBack({ 0, 672, 304, 224 });
	bgLawnAnim.speed = 0.2f;

	//Load concrete bg animation
	bgConcreteAnim.PushBack({ 0, 0, 304, 224 });
	bgConcreteAnim.PushBack({ 0, 224, 304, 224 });
	bgConcreteAnim.PushBack({ 0, 448, 304, 224 });
	bgConcreteAnim.PushBack({ 0, 672, 304, 224 });
	bgConcreteAnim.speed = 0.2f;

	
	for (int i = 5; i >= 0; i--) {
		lanzamientoIzquierda.PushBack({ 0 + (i * 56), 190, 56, 38 });
	}
	for (int i = 5; i >= 0; i--) {
		lanzamientoIzquierda.PushBack({ 0 + (i * 56), 152, 56, 38 });
	}
	for (int i = 5; i >= 0; i--) {
		lanzamientoIzquierda.PushBack({ 0 + (i * 56), 114, 56, 38 });
	}
	lanzamientoIzquierda.loop = false;
	lanzamientoIzquierda.speed = 0.2f;
		
	for (int i = 5; i >= 0; i--) {
		lanzamientoDerecha.PushBack({ 0 + (i * 56), 190, 56, 38 });
	}
	for (int i = 3; i >= 0; i--) {
		lanzamientoDerecha.PushBack({ 112 + (i * 56), 152, 56, 38 });
	}
	for (int i = 5; i >= 0; i--) {
		lanzamientoDerecha.PushBack({ 0 + (i * 56), 76, 56, 38 });
	}
	lanzamientoDerecha.PushBack({ 280, 38, 56, 38 });
	lanzamientoDerecha.loop = false;
	lanzamientoDerecha.speed = 0.2f;
		
	scoreP1.PushBack({ 59, 40, 56, 38 });
	scoreP1.PushBack({ 0, 40, 56, 38 });
	scoreP1.PushBack({ 280, 0, 56, 38 });
	scoreP1.PushBack({ 223, 0, 56, 38 });
	scoreP1.loop = false;
	scoreP1.speed = 0.1f;
	
	scoreP2.PushBack({ 168, 0, 56, 38 });
	scoreP2.PushBack({ 55, 0, 56, 38 });
	scoreP2.PushBack({ 115, 0, 56, 38 });
	scoreP2.PushBack({ 0, 0, 56, 38 });
	scoreP2.loop = false;
	scoreP2.speed = 0.1f;
	
	miradaIzq.PushBack({ 224, 38, 56, 38 });
	miradaIzq.loop = false;
	
	miradaDerecha.PushBack({ 112, 38, 56, 38 });
	miradaDerecha.loop = false;

	quieto.PushBack({ 168,38,56,38 });
	quieto.loop = false;



}

SceneBeachStage::~SceneBeachStage()
{

}

// Load assets
bool SceneBeachStage::Start()
{

	if (loadFxOnce == 0) {
		round1FX = 0;

		//FX de rounds
		round1FX = App->audio->LoadFx("Assets/Fx/round1.wav");
		twoPointsFX = App->audio->LoadFx("Assets/Fx/2Pts.wav");
		threePointsFX = App->audio->LoadFx("Assets/Fx/3Pts.wav");
		fivePointsFX = App->audio->LoadFx("Assets/Fx/5Pts.wav");

		applauseFX = App->audio->LoadFx("Assets/Fx/Applause.wav");
		bigApplauseFX = App->audio->LoadFx("Assets/Fx/BigApplause.wav");
		excelentGameFX = App->audio->LoadFx("Assets/Fx/ExcelentGame.wav");
		finalRoundFX = App->audio->LoadFx("Assets/Fx/FinalRound.wav");
		getReadyFX = App->audio->LoadFx("Assets/Fx/GetReady.wav");
		goalHitFX = App->audio->LoadFx("Assets/Fx/GoalHit.wav");

		whistleFX = App->audio->LoadFx("Assets/Fx/Whistle.wav");

		switch (App->sceneCharacterSelect->p1Char)
		{
		case Mita:
			celebrationFX = App->audio->LoadFx("Assets/Fx/HiromiSetWin.wav");
			break;
		case Yoo:
			celebrationFX = App->audio->LoadFx("Assets/Fx/YooSetWin.wav");
			break;
		case Wessel:
			celebrationFX = App->audio->LoadFx("Assets/Fx/KlaussSetWin.wav");
			break;
		}
	}
	currentAnimationFrisbee = &quieto;

	time = 0;
	LOG("Loading background assets");

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	bool ret = true;

	suddenDeath = false;
	initialTime = 0;
	startTheGame = false;
	stopCelebration = false;
	getReadyOnce = true;

	if (App->sceneStageSelect->sceneSelected == Beach) {
		texturaArbitro = App->textures->Load("Assets/Sprites/Arbitro.png");
	}
	else if (App->sceneStageSelect->sceneSelected == Lawn) {
		texturaArbitro = App->textures->Load("Assets/Sprites/ArbitroFM.png");
	}
	else if (App->sceneStageSelect->sceneSelected == Concrete) {
		texturaArbitro = App->textures->Load("Assets/Sprites/ArbitroFB.png");
	}

	estadoGolScore = CLEAR;


	//currentAnimationFrisbee = &lanzamientoIzquierda;

	//Canviar musica depenent de l'escenari
	switch (App->sceneStageSelect->sceneSelected)
	{
	case Beach:
		bgBeachTexture = App->textures->Load("Assets/Sprites/Levels/bgBeachSpriteSheet.png");
		bgBeachObstacles = App->textures->Load("Assets/Sprites/Levels/bgBeachObstacles.png");
		
		currentBgAnim = &bgBeachAnim;
		currentAnimationFrisbee = &quieto;

		App->audio->PlayMusic("Assets/Music/03_Flying Power Disc (Beach Court).ogg", 1.0f);

		limiteCentralIzq = 110;
		limiteIzquierda = 10;
		limiteSuperior = 50;
		limiteInferior = 138;
		limiteCentralDer = 145;
		limiteDerecha = 240;
		posicionXInicialFrisbee = 130;
		posicionYInicialFrisbee = 177;


		break;
	case Lawn:
		bgLawnTexture = App->textures->Load("Assets/Sprites/Levels/bgLawnSpriteSheet.png");
		bgLawnObstacles = App->textures->Load("Assets/Sprites/Levels/bgLawnObstacles.png");
		
		currentAnimationFrisbee = &quieto;

		currentBgAnim = &bgLawnAnim;

		//CANVIAR LA MUSICA A LA CORRESPONENT DE L'ESCENARI
		App->audio->PlayMusic("Assets/Music/05_Windjammers (Lawn Court).ogg", 1.0f);

		limiteCentralIzq = 110;
		limiteIzquierda = 10;
		limiteSuperior = 25;
		limiteInferior = 153;
		limiteCentralDer = 140;
		limiteDerecha = 248;
		posicionXInicialFrisbee = 130;
		posicionYInicialFrisbee = 177;

		break;
	case Concrete:
		bgConcreteTexture = App->textures->Load("Assets/Sprites/Levels/bgConcreteSpriteSheet.png");
		bgConcreteObstacles = App->textures->Load("Assets/Sprites/Levels/bgConcreteObstacles.png");
		
		currentAnimationFrisbee = &quieto;

		currentBgAnim = &bgConcreteAnim;

		//CANVIAR LA MUSICA A LA CORRESPONENT DE L'ESCENARI
		App->audio->PlayMusic("Assets/Music/04_You-got-a-power-_Concrete_.ogg", 1.0f);

		limiteCentralIzq = 100;
		limiteIzquierda = 10;
		limiteSuperior = 20;
		limiteInferior = 158;
		limiteCentralDer = 150;
		limiteDerecha = 244;
		posicionXInicialFrisbee = 130;
		posicionYInicialFrisbee = 177;

		break;
	}

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->player2->Enable();
	App->frisbee->Enable();
	App->collisions->Enable();
	App->ingameUI->Enable();

	roundSpriteAppear = false;
	setsSpriteAppear = false;

	debugwinP1 = false;
	debugwinP2 = false;
	winState = 0;
	godMode = false;
	estadoS = INICIO;
	estadoTS = INICIOT;
	estadoTGol == INICIOGOL;
	arbitroFinalRonda = 1;

	return ret;
}

Update_Status SceneBeachStage::Update()
{

	/*if (_kbhit() && _getch() == 27)
		return Update_Status::UPDATE_STOP;*/
	
	switch (estadoS) {
		//4 segons inicicials (puntuacions, ui general)
	case (INICIO):

		//bullshit animaciones texturas etc - inicio partida, primer momento, solo ocurre una vez en cada partida
		if (estadoTS == INICIOT)
		{
			App->audio->PlayFx(round1FX);
			initialTimeS = SDL_GetTicks();
			timeLimitS = 4 * 1000;
			estadoTS = EJECUTANDO;
		}
		else if (estadoTS == EJECUTANDO) {
			/*setsSpriteAppear = true;*/
			TimerS();
		}
		else if (estadoTS == FIN)
		{
			//setsSpriteAppear = false;
			//SceneBeachStage::Arbitro(1);
			estadoTS = INICIOT;
			estadoS = INICIORONDA;
		}
		break;

		//Iniciar ronda (round 2, final o suddendeath)
	case (INICIORONDA):
		//Animacion Ronda 1.
		App->player->score = 0;
		App->player2->score = 0;
		App->player->position.x = App->player->posicionInicialX;
		App->player->position.y = App->player->posicionInicialY;
		App->player2->position.x = App->player2->posicionInicialX;
		App->player2->position.y = App->player2->posicionInicialY;

		if (estadoTS == INICIOT)
		{
			/*App->audio->PlayFx(getReadyFX);*/
			initialTimeS = SDL_GetTicks();
			if (App->player->round == 0 && App->player2->round == 0)
			{
				timeLimitS = 1;
			}
			else
			{
				timeLimitS = 2 * 1000;
			}
			estadoTS = EJECUTANDO;
		}
		else if (estadoTS == EJECUTANDO) {
			roundSpriteAppear = true;
			if (((App->player->round >= 1 && App->player->round < 2)|| (App->player2->round >= 1 && App->player2->round < 2)) && getReadyOnce)
			{
				App->audio->PlayFx(getReadyFX);
				getReadyOnce = false;
			}

			TimerS();
		}
		else if (estadoTS == FIN)
		{
			App->audio->PlayFx(whistleFX);
			estadoTS = INICIOT;
			estadoS = RONDA;
			App->frisbee->estadoF = App->frisbee->LANZAMIENTOARBITRO;
			getReadyOnce = true;
		}

		break;

		//INGAME Gemplei 
	case (RONDA):
		App->ingameUI->currentTimerAnim->Update();
		if (estadoTS == INICIOT) {
			roundSpriteAppear = false;
			initialTimeS = SDL_GetTicks();
			timeLimitS = 30 * 1000;
			App->frisbee->estadoF = App->frisbee->LANZAMIENTOARBITRO;
			estadoTS = EJECUTANDO;
			App->ingameUI->timerAnim.Reset();
		}
		//if (estadoTGol == INICIOGOL) {
		//SI NO FUNCIONA, TREURE ROUND();
		//Round();
		// -----------------------------------------------------------------------------
		//}//Tendremos que poner una condicion para cuando se marquen puntos que aqui se ejecuten unas texuras/animaciones - MARCARPUNTO
		if (estadoTGol == EJECUTANDOGOL)
		{
			TimerGol();
		}
		else if (estadoTGol == FINGOL)
		{
			App->player->position.x = App->player->posicionInicialX;
			App->player->position.y = App->player->posicionInicialY;
			App->player2->position.x = App->player2->posicionInicialX;
			App->player2->position.y = App->player2->posicionInicialY;
			if (suddenDeath) {
				Win();
			}
			estadoTGol = INICIOGOL;
			Round();
			if (estadoS != FINALRONDA) {
				App->frisbee->estadoF = App->frisbee->LANZAMIENTOARBITRO;
			}
		}
		else if (estadoTS == EJECUTANDO) {
			TimerS();
			if (App->frisbee->estadoF != App->frisbee->LANZAMIENTOARBITRO) {
				miradaArbitro();
			}

		}
		else if (estadoTS == FIN && App->frisbee->estadoF != App->frisbee->MOVIMIENTO && App->frisbee->estadoF != App->frisbee->BLOCK) {
			Round();
			estadoTS = INICIOT;

		}
		break;

		//Animacions qui ha guanyat bailecito chingon
	case (FINALRONDA):
		//Animaciones caundo se gana/pierde una ronda
		//App->player->estadoP1 = App->player->STOP;
		//App->player2->estadoP2 = App->player2->STOP;
		Win();

		if (estadoTS == INICIOT)
		{
			App->frisbee->limiteConcrete = false;
			initialTimeS = SDL_GetTicks();
			timeLimitS = 4 * 1000;
			estadoTS = EJECUTANDO;
		}
		else if (estadoTS == EJECUTANDO) {
			setsSpriteAppear = true;
			TimerS();
		}
		else if (estadoTS == FIN)
		{
			setsSpriteAppear = false;
			estadoS = INICIORONDA;
			estadoTS = INICIOT;
		}
		break;

		//Animacions qui ha guanyat bailecito chingon i reiniciar
	case (FINAL):
		App->player->estadoP1 = App->player->STOP;
		App->player2->estadoP2 = App->player2->STOP;
		App->frisbee->estadoF = App->frisbee->ARBITROF;
		if (estadoTS == INICIOT)
		{
			initialTimeS = SDL_GetTicks();
			timeLimitS = 4 * 1000;
			estadoTS = EJECUTANDO;
		}
		if (estadoTS == EJECUTANDO)
		{
			//App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
			TimerS();

		}
		if (estadoTS == FIN)
		{
			//SceneBeachStage::Arbitro(1);
			estadoS = INICIO;
			App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
			App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		}
		break;

	}
	
	// DEBUG INSTANT WIN
	if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN)
	{
		estadoTS = INICIOT;
		debugwinP1 = true;
		Win();
	}
	// DEBUG INSTANT LOSE
	if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN)
	{
		estadoTS = INICIOT;
		debugwinP2 = true;
		Win();
	}
	if (App->input->keys[SDL_SCANCODE_F1] == Key_State::KEY_DOWN)
	{
		if (estadoS == RONDA) {
			godMode = true;
		}
	}
	if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
	{
		if (!isDebugAppear)
			isDebugAppear = true;
		else isDebugAppear = false;
	}

	currentAnimationFrisbee->Update();
	currentBgAnim->Update(); 

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneBeachStage::PostUpdate()
{
	SDL_Rect rect = currentAnimationFrisbee->GetCurrentFrame();
	backgroundAnimationRect = currentBgAnim->GetCurrentFrame();
	
	switch (App->sceneStageSelect->sceneSelected)  
	{
	case Beach:
		App->render->Blit(bgBeachTexture, 0, 0, &backgroundAnimationRect);
		App->render->Blit(bgBeachObstacles, 0, 0, NULL);
		App->render->Blit(texturaArbitro, 127, 175, &rect);

		break;
	case Lawn:
		App->render->Blit(bgLawnTexture, 0, 0, &backgroundAnimationRect);
		App->render->Blit(bgLawnObstacles, 0, 0, NULL);
		App->render->Blit(texturaArbitro, 127, 190, &rect);

		break;
	case Concrete:
		App->render->Blit(bgConcreteTexture, 0, 0, &backgroundAnimationRect);
		App->render->Blit(bgConcreteObstacles, 0, 0, NULL);
		App->render->Blit(texturaArbitro, 127, 190, &rect);

		break;
	}

	if (estadoS == FINAL && !stopCelebration)
	{
		App->audio->PlayFx(celebrationFX);
		stopCelebration = true;
	}

	

	if (App->input->keys[SDL_SCANCODE_F6] == Key_State::KEY_DOWN)
	{
		App->particles->AddParticle(0, 0, App->particles->mitaSuperShotParticle, 110, 120, Collider::NONE, 0);
		//App->audio->PlayFx(goalHitFX);
	}

	if (isDebugAppear)
	{
	/*	sprintf_s(debugText, 10, "%2d", (int)App->frisbee->position.x - ((int)App->player->position.x + 20));
		
		sprintf_s(debugText2, 10, "%2d", (int)App->frisbee->position.y);*/

		sprintf_s(debugText, 10, "%2d", (int)App->frisbee->position.x);

		sprintf_s(debugText2, 10, "%2d", (int)App->frisbee->position.y);
		
		//DEBUGG ESTADO PLAYER1
		if (App->player->estadoP1 == 0) {
			App->fonts->BlitText(30, 50, debugFont, "P1.STOP");
		}
		else if (App->player->estadoP1 == 1) {
			App->fonts->BlitText(30, 50, debugFont, "P1.MOVIMIENTO");
		}
		else if (App->player->estadoP1 == 2) {
			App->fonts->BlitText(30, 50, debugFont, "P1.WIITHFRISBEE");
		}

		//DEBUGG ESTADO PLAYER2
		if (App->player2->estadoP2 == 0) {
			App->fonts->BlitText(160, 50, debugFont, "P2.STOP");
		}
		else if (App->player2->estadoP2 == 1) {
			App->fonts->BlitText(160, 50, debugFont, "P2.MOVIMIENTO");
		}
		else if (App->player2->estadoP2 == 2) {
			App->fonts->BlitText(160, 50, debugFont, "P2.WIITHFRISBEE");
		}

		//DEBUGG ESTADO FRISBEE


		if (App->frisbee->estadoF == 0) {
			App->fonts->BlitText(110, 100, debugFont, "ARBITROF");
		}
		else if (App->frisbee->estadoF == 1) {
			App->fonts->BlitText(110, 100, debugFont, "STOP");
		}
		else if (App->frisbee->estadoF == 2) {
			App->fonts->BlitText(110, 100, debugFont, "MOVIMIENTO");
		}
		else if (App->frisbee->estadoF == 3) {
			App->fonts->BlitText(110, 100, debugFont, "PRUEBA");
		}
		else if (App->frisbee->estadoF == 4) {
			App->fonts->BlitText(110, 100, debugFont, "WITHPLAYER");
		}
		else if (App->frisbee->estadoF == 5) {
			App->fonts->BlitText(110, 100, debugFont, "BLOCK");
		}
		else if (App->frisbee->estadoF == 6) {
			App->fonts->BlitText(110, 100, debugFont, "SUELO");
		}

		//DEBUG ESTADO TIMER SCENE BEACH STAGE

		if (estadoTS == 0) {
			App->fonts->BlitText(95, 33, debugFont, "TS.INICIOT");
		}
		else if (estadoTS == 1) {
			App->fonts->BlitText(95, 33, debugFont, "TS.EJECUTANDO");
		}
		else if (estadoTS == 2) {
			App->fonts->BlitText(95, 33, debugFont, "TS.FIN");
		}

		if (estadoS == 0){
			App->fonts->BlitText(110, 180, debugFont, "S.INICIO");
		}
		if (estadoS == 1) {
			App->fonts->BlitText(110, 180, debugFont, "S.INICIORONDA");
		}
		if (estadoS == 2) {
			App->fonts->BlitText(110, 180, debugFont, "S.RONDA");
		}
		if (estadoS == 3) {
			App->fonts->BlitText(110, 180, debugFont, "S.FINALRONDA");
		}
		if (estadoS == 4) {
			App->fonts->BlitText(110, 180, debugFont, "S.FINAL");
		}

		if (App->frisbee->lanzamientoF == 0) {
			App->fonts->BlitText(110, 120, debugFont, "NORMAL");
		}
		else if (App->frisbee->lanzamientoF == 1) {
			App->fonts->BlitText(110, 120, debugFont, "PARABOLA");
		}
		else if (App->frisbee->lanzamientoF == 2) {
			App->fonts->BlitText(110, 120, debugFont, "ARBITRO");
		}
		else if (App->frisbee->lanzamientoF == 3) {
			App->fonts->BlitText(110, 120, debugFont, "SUPERSHOT");
		}
		else if (App->frisbee->lanzamientoF == 4) {
			App->fonts->BlitText(110, 120, debugFont, "BLOCKPLAYER1");
		}
		else if (App->frisbee->lanzamientoF == 5) {
			App->fonts->BlitText(110, 120, debugFont, "BLOCKPLAYER2");
		}


		if(App->frisbee->estadoTF == 0) {
			App->fonts->BlitText(110, 150, debugFont, "INICIO");
		}
		else if (App->frisbee->estadoTF == 1) {
			App->fonts->BlitText(110, 150, debugFont, "EJECUTANDO");
		}
		else if (App->frisbee->estadoTF == 2) {
			App->fonts->BlitText(110, 150, debugFont, "FIN");
		}


		App->fonts->BlitText(110, 110, debugFont, debugText);
		App->fonts->BlitText(165, 110, debugFont, debugText2);
		
	}

	//BLIT 5 punts a ModuleInGameUI.cpp

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneBeachStage::CleanUp()
{
	App->player->Disable();
	App->player2->Disable();
	App->frisbee->Disable();
	App->collisions->Disable();
	App->ingameUI->Disable();

	App->textures->Unload(bgBeachTexture);
	App->textures->Unload(bgLawnTexture);
	App->textures->Unload(bgConcreteTexture);
	App->textures->Unload(bgBeachObstacles);
	App->textures->Unload(bgLawnObstacles);
	App->textures->Unload(bgConcreteObstacles);
	App->textures->Unload(timerTexture);
	App->textures->Unload(uiSpriteTexture);
	App->textures->Unload(texturaArbitro);
	App->textures->Unload(Winn);
	App->fonts->UnLoad(debugFont);

	loadFxOnce = 1;
	App->sceneTitle->loadFxOnceTitle = 1;
	//App->fonts->UnLoad(debugFont);

	return true;
}

void SceneBeachStage::Arbitro(int arbitro) { 
	App->player->currentAnimation = &App->player->idleRAnim;
	App->player2->currentAnimation = &App->player2->idleLAnim;

	if (arbitro == 1) {
		App->frisbee->xspeed = -3; 
		App->frisbee->yspeed = -2;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
		App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::ARBITRO;


	}
	else if (arbitro == 2) {
		App->frisbee->xspeed = 3;
		App->frisbee->yspeed = -2;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
		App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::ARBITRO;
	}
}

void SceneBeachStage::Round() {
	if (!godMode) {

		if (App->player->score >= 12 || App->player2->score >= 12) {

			if (App->player->score > App->player2->score + 2) {
				App->player->round += 1;
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Llamar animaci�n de jugador ganador 1 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				arbitroFinalRonda = 2;
				estadoTS = INICIOT;
				estadoS = FINALRONDA;

			}

			if (App->player2->score > App->player->score + 2) {
				App->player2->round += 1;
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;
				//Llamar animaci�n de jugador ganador 2 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				arbitroFinalRonda = 1;
				estadoTS = INICIOT;
				estadoS = FINALRONDA;
			}

		}
		else if (estadoTS == FIN) { //FALTA TIMER
			App->frisbee->position.x = posicionXInicialFrisbee;
			App->frisbee->position.y = posicionYInicialFrisbee;
			App->player->estadoP1 = ModulePlayer::estadoPlayer::STOP;
			App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::STOP;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::ARBITROF;


			if (App->player->score > App->player2->score) {
				App->player->round += 1;
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Llamar animacion de jugador ganador 1 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				arbitroFinalRonda = 2;
				estadoS = FINALRONDA;

			}
			else if (App->player2->score > App->player->score) {
				App->player2->round += 1;
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;

				//Llamar animaci�n de jugador ganador 2 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				arbitroFinalRonda = 1;
				estadoS = FINALRONDA;

			}
			else if (App->player->score == App->player2->score) {
				App->player->round += 1;
				App->player2->round += 1;
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->win;

				App->player->score = 0;
				App->player2->score = 0;
				arbitroFinalRonda = 1;
				estadoS = FINALRONDA;

				//Animaci�n de cuando los dos acaban una ronda en puntuacion empate

			}

		}

		//Si es guanya de pallisa play ExcelentGameFx
		if ((App->player->round == 2 && App->player2->round == 0) || ((App->player->round == 0 && App->player2->round == 2)))
		{
			App->audio->PlayFx(excelentGameFX);
		}

	}

}

void SceneBeachStage::Win() { 

	if (App->player->round == App->player2->round && App->player->round == 2 && App->player2->round == 2 && !suddenDeath) {
		suddenDeath = true;
		estadoS = INICIORONDA;
	}
	else if (App->player->score != 0 && suddenDeath) {
		//llamar animaci�n y texturas de que ha ganado el primer jugador la partida
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		winState = 1;
		estadoS = FINAL;
		App->player->currentAnimation = &App->player->win;
		App->player2->currentAnimation = &App->player2->lose;
		currentAnimationFrisbee = &scoreP1;

	}
	else if (App->player2->score != 0 && suddenDeath) {
		//llamar animaci�n y texturas de que ha ganado el segundo jugador la partida
		winState = 2;
		estadoS = FINAL;
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		App->player->currentAnimation = &App->player->lose;
		App->player2->currentAnimation = &App->player2->win;
		currentAnimationFrisbee = &scoreP2;

	}
	else if ((App->player->round == 2 && !suddenDeath) || debugwinP1) {
		//llamar animaci�n y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		winState = 1;
		estadoS = FINAL;
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		App->player->currentAnimation = &App->player->win;
		App->player2->currentAnimation = &App->player2->lose;
		currentAnimationFrisbee = &scoreP1;
	}
	else if ((App->player2->round == 2 && !suddenDeath) || debugwinP2) {
		//llamar animaci�n y texturas de que ha ganado el segundo jugador la partida
		winState = 2;
		estadoS = FINAL;
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		App->player->currentAnimation = &App->player->lose;
		App->player2->currentAnimation = &App->player2->win;
		currentAnimationFrisbee = &scoreP2;

	}
	else if (suddenDeath && App->player->score == App->player2->score) {
		//Animacion y texturas de que los dos han perdido
		winState = 3;
		estadoS = FINAL;
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		App->player->currentAnimation = &App->player->lose;
		App->player2->currentAnimation = &App->player2->lose;

	}
	else if (!godMode) {
		//estadoS = INICIORONDA;
	}
}

void SceneBeachStage::Score(){
	App->player->estadoP1 = ModulePlayer::estadoPlayer::STOP;
	App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::STOP;
	currentAnimationFrisbee->Reset();
	if (App->sceneStageSelect->sceneSelected == Concrete)
	{
		//Score de Concrete
		//Esquerra
		if (App->frisbee->position.x <= 19) {
			App->particles->AddParticle(0, 0, App->particles->leftGoalFlashParticle, App->frisbee->position.x - 5, App->frisbee->position.y, Collider::NONE, 0);
			App->audio->PlayFx(goalHitFX);
			//Mid left
			if (App->frisbee->position.y >= 71 && App->frisbee->position.y <= 184) {
				App->player2->score += 3;
				App->audio->PlayFx(threePointsFX);
				App->audio->PlayFx(applauseFX);
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP1;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 1;
				estadoGolScore = MIDLEFT;
			}
			//Upleft
			else if (App->frisbee->position.y < 71)
			{

				App->player2->score += 5;
				App->audio->PlayFx(fivePointsFX);
				App->audio->PlayFx(bigApplauseFX);
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 1;
				estadoGolScore = UPLEFT;
			}
			//down left
			else if (App->frisbee->position.y > 184)
			{
				App->player2->score += 5;
				App->audio->PlayFx(fivePointsFX);
				App->audio->PlayFx(bigApplauseFX);
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 1;
				estadoGolScore = DOWNLEFT;
			}
		}
		//Dreta
		else if (App->frisbee->position.x >= 276) {
			App->particles->AddParticle(0, 0, App->particles->rightGoalFlashParticle, App->frisbee->position.x - 10, App->frisbee->position.y, Collider::NONE, 0);
			App->audio->PlayFx(goalHitFX);
			//mid right
			if (App->frisbee->position.y >= 71 && App->frisbee->position.y <= 184) {
				App->player->score += 3;
				App->audio->PlayFx(threePointsFX);
				App->audio->PlayFx(applauseFX);
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 2;
				estadoGolScore = MIDRIGHT;
			}
			// upright
			else if (App->frisbee->position.y < 71)
			{
				App->player->score += 5;
				App->audio->PlayFx(fivePointsFX);
				App->audio->PlayFx(bigApplauseFX);
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 2;
				estadoGolScore = UPRIGHT;
			}
			//down right
			else if (App->frisbee->position.y > 184)
			{
				App->player->score += 5;
				App->audio->PlayFx(fivePointsFX);
				App->audio->PlayFx(bigApplauseFX);
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;


				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 2;
				estadoGolScore = DOWNRIGHT;
			}
		}
		// miss
		else if (App->frisbee->position.x > 19 && App->frisbee->position.x < 150) {
			App->player2->score += 2;
			App->player->currentAnimation = &App->player->lose;
			App->player2->currentAnimation = &App->player2->win;
			currentAnimationFrisbee = &scoreP2;
			App->audio->PlayFx(twoPointsFX);
			if (suddenDeath) {
				Win();
			}

			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 1;
			estadoGolScore = MISSL;
		}
		else if (App->frisbee->position.x < 276 && App->frisbee->position.x > 150) {
			App->player->score += 2;
			App->player->currentAnimation = &App->player->win;
			App->player2->currentAnimation = &App->player2->lose;
			currentAnimationFrisbee = &scoreP1;
			App->audio->PlayFx(twoPointsFX);
			if (suddenDeath) {
				Win();
			}
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 2;
			estadoGolScore = MISSR;
		}
	}
	else
	{
	//Score de Beach i Lawn
	//Esquerra
		if (App->frisbee->position.x <= 19) {
			App->particles->AddParticle(0, 0, App->particles->leftGoalFlashParticle, App->frisbee->position.x - 5, App->frisbee->position.y, Collider::NONE, 0);
			App->audio->PlayFx(goalHitFX);
			//Mid left
			if (App->frisbee->position.y >= 94 && App->frisbee->position.y <= 144) {
				App->player2->score += 5;
				App->audio->PlayFx(fivePointsFX);
				App->audio->PlayFx(bigApplauseFX);
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 1;
				estadoGolScore = MIDLEFT;
			}
			//Upleft
			else if (App->frisbee->position.y < 94)
			{
				App->player2->score += 3;
				App->audio->PlayFx(threePointsFX);
				App->audio->PlayFx(applauseFX);
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 1;
				estadoGolScore = UPLEFT;
			}
			//down left
			else if (App->frisbee->position.y > 144)
			{
				App->player2->score += 3;
				App->audio->PlayFx(threePointsFX);
				App->audio->PlayFx(applauseFX);
				App->player->currentAnimation = &App->player->lose;
				App->player2->currentAnimation = &App->player2->win;
				currentAnimationFrisbee = &scoreP2;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 1;
				estadoGolScore = DOWNLEFT;
			}
		}
		//Dreta
		else if (App->frisbee->position.x >= 276) {
			App->particles->AddParticle(0, 0, App->particles->rightGoalFlashParticle, App->frisbee->position.x - 10, App->frisbee->position.y, Collider::NONE, 0);
			App->audio->PlayFx(goalHitFX);

			//mid right
			if (App->frisbee->position.y >= 94 && App->frisbee->position.y <= 144) {
				App->player->score += 5;
				App->audio->PlayFx(fivePointsFX);
				App->audio->PlayFx(bigApplauseFX);
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 2;
				estadoGolScore = MIDRIGHT;
			}
			// upright
			else if (App->frisbee->position.y < 94)
			{
				App->player->score += 3;
				App->audio->PlayFx(threePointsFX);
				App->audio->PlayFx(applauseFX);
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 2;
				estadoGolScore = UPRIGHT;
			}
			//down right
			else if (App->frisbee->position.y > 144)
			{
				App->player->score += 3;
				App->audio->PlayFx(threePointsFX);
				App->audio->PlayFx(applauseFX);
				App->player->currentAnimation = &App->player->win;
				App->player2->currentAnimation = &App->player2->lose;
				currentAnimationFrisbee = &scoreP1;

				//Just despres d'afegir score, UI Textura d'on ha marcat
				if (suddenDeath) {
					Win();
				}
				initialTimeGol = SDL_GetTicks();
				timeLimitGol = 2 * 1000;
				estadoTGol = EJECUTANDOGOL;
				arbitroFinalRonda = 2;
				estadoGolScore = DOWNRIGHT;
			}
		}
		// miss
		else if (App->frisbee->position.x > 19 && App->frisbee->position.x < 140) {
			App->player2->score += 2;
			App->player->currentAnimation = &App->player->lose;
			App->player2->currentAnimation = &App->player2->win;
			currentAnimationFrisbee = &scoreP2;
			App->audio->PlayFx(twoPointsFX);
			if (suddenDeath) {
				Win();
			}

			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 1;
			estadoGolScore = MISSL;
		}
		else if (App->frisbee->position.x < 276 && App->frisbee->position.x > 140) {
			App->player->score += 2;
			App->player->currentAnimation = &App->player->win;
			App->player2->currentAnimation = &App->player2->lose;
			currentAnimationFrisbee = &scoreP1;
			App->audio->PlayFx(twoPointsFX);
			if (suddenDeath) {
				Win();
			}
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 2;
			estadoGolScore = MISSR;
		}
	}

	App->frisbee->position.x = posicionXInicialFrisbee;
	App->frisbee->position.y = posicionYInicialFrisbee;
}

void SceneBeachStage::TimerS() {
	currentTimeS = SDL_GetTicks();

	if (currentTimeS - initialTimeS >= timeLimitS) {
		estadoTS = estadoTimerS::FIN;
	}
}

void SceneBeachStage::TimerGol() {
	currentTimeGol = SDL_GetTicks();

	if (currentTimeGol - initialTimeGol >= timeLimitGol) {
		estadoTGol = estadoTimerGol::FINGOL;

		//Elimina textura quan estadoTGol sigui FINGOL
		estadoGolScore = CLEAR;
	}

}


void SceneBeachStage::miradaArbitro() {
	currentAnimationFrisbee->Reset();

	if (App->frisbee->position.x <= 120) { //limiteMedio - hasta donde el cual el arbitro mira izquierda, pasada esta posicion --> derecha
		currentAnimationFrisbee = &miradaIzq;
	} 
	else if (App->frisbee->position.x >=130 ) { //limiteMedio - ''    ''
		currentAnimationFrisbee = &miradaDerecha;
	}
	
}