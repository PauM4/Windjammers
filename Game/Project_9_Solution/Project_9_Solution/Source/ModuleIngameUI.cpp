#include "ModuleInGameUI.h"

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
#include "ModuleFonts.h"
#include "SceneBeachStage.h"
#include "SceneCharacterSelect.h"
#include "SceneStageSelect.h"

#include "SDL/include/SDL.h"

#include <stdio.h>


ModuleInGameUI::ModuleInGameUI(bool startEnabled) : Module(startEnabled)
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

}

ModuleInGameUI::~ModuleInGameUI()
{

}

// Load assets
bool ModuleInGameUI::Start()
{
	bool ret = true;
	punterParabolaP = { 0, 0, 15, 15 };

	rectanguletL = { 0, 357, 29, 3 };
	rectanguletR = { 0, 357, 29, 3 };

	winUIRight = { 224, 86, 100, 32 };
	LoseUIRight = { 393, 27, 99, 27 };

	winUILeft = { 324, 86, 112, 32 };
	LoseUILeft = { 0, 54, 100, 27 };

	drawGameRect = { 100, 54, 212, 29 };

	tresPuntsL = { 0, 188, 63, 34 };
	cincPuntsL = { 76, 258, 63, 35 };

	tresPuntsR = { 112, 120, 63, 34 };
	cincPuntsR = { 74, 224, 63, 35 };

	dosPunts = { 139, 258, 40, 41 };

	set1Rect = { 160, 300, 160, 56 };
	set2Rect = { 320, 300, 160, 56 };
	suddenRect = { 179, 258, 270, 42 };
	setFinalRect = { 0, 300, 160, 56 };
	rectTimer88 = { 0, 0, 15, 15 };

	rectNormes = { 207, 11, 151, 15 };

	p1Rect = { 359, 0, 15, 8 };
	p2Rect = { 374, 0, 16, 8 };

	japanFlagRect = { 460, 0, 15, 9 };
	koreanFlagRect = { 430, 0, 15, 9 };
	germanyFlagRect = { 445, 0, 15, 9 };

	setCountRect = { 315, 27, 78, 26 };
	setNum0 = { 135, 27, 45, 26 };
	setNum1 = { 180, 27, 45, 26 };
	setNum2 = { 225, 27, 45, 26 };
	setNum3 = { 270, 27, 45, 26 };

	rounds = { 0,0,0,0 };

	ambQuadradet = true;
	verticalPos = 107;

	score = 000;
	scoreFont = -1;

	score2 = 000;
	scoreFont2 = -1;

	char lookupTable1[] = { "0123456789G " };
	scoreFont = App->fonts->Load("Assets/Sprites/UI/Fonts/scoreFont.png", lookupTable1, 1);

	char lookupTable2[] = { "0123456789G " };
	scoreFont2 = App->fonts->Load("Assets/Sprites/UI/Fonts/scoreFont.png", lookupTable2, 1);

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	// Load UI Texture
	uiSpriteTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");
	bothCharactersTexture = App->textures->Load("Assets/Sprites/UI/charactersPresent2.png");

	// Load timer texture
	timerTexture = App->textures->Load("Assets/Sprites/UI/Fonts/timerSpriteSheet.png");
	currentTimerAnim = &timerAnim;

	// Load rectangulet Lila per set count
	rectanguletLila = App->textures->Load("Assets/Sprites/UI/rectanguletLila.png");

	//Punter parabola
	punterParabola = App->textures->Load("Assets/Sprites/UI/punterParabola.png");

	//P1 Left
	switch (App->sceneCharacterSelect->p1Char)
	{
	case Mita:
		leftCharTxt = App->textures->Load("Assets/Sprites/UI/Faces/mitaLeft.png");
		leftCharLostTxt = App->textures->Load("Assets/Sprites/UI/Faces/mitaLostLeft.png");
		break;
	case Yoo:
		leftCharTxt = App->textures->Load("Assets/Sprites/UI/Faces/yooLeft.png");
		leftCharLostTxt = App->textures->Load("Assets/Sprites/UI/Faces/yooLostLeft.png");
		break;
	case Wessel:
		leftCharTxt = App->textures->Load("Assets/Sprites/UI/Faces/wesselLeft.png");
		leftCharLostTxt = App->textures->Load("Assets/Sprites/UI/Faces/wesselLostLeft.png");
		break;
	}

	//P2 Right
	switch (App->sceneCharacterSelect->p2Char)
	{
	case Mita:
		rightCharTxt = App->textures->Load("Assets/Sprites/UI/Faces/mitaRight.png");
		rightCharLostTxt = App->textures->Load("Assets/Sprites/UI/Faces/mitaLostRight.png");
		break;
	case Yoo:
		rightCharTxt = App->textures->Load("Assets/Sprites/UI/Faces/yooRight.png");
		rightCharLostTxt = App->textures->Load("Assets/Sprites/UI/Faces/yooLostRight.png");
		break;
	case Wessel:
		rightCharTxt = App->textures->Load("Assets/Sprites/UI/Faces/wesselRight.png");
		rightCharLostTxt = App->textures->Load("Assets/Sprites/UI/Faces/wesselLostRight.png");
		break;
	}

	return ret;
}

Update_Status ModuleInGameUI::Update()
{
	if (App->frisbee->estadoF == App->frisbee->SUELO) {
		posXFrisbeeMiss = App->frisbee->position.x;
		posYFrisbeeMiss = App->frisbee->position.y-16;
	}
	
	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status ModuleInGameUI::PostUpdate()
{
	//Quadradet final d'on caura la parabola
	if(App->frisbee->lanzamientoF == App->frisbee->PARABOLA && App->frisbee->estadoF == App->frisbee->MOVIMIENTO){
		App->render->Blit(punterParabola, App->frisbee->parabolaFinalX+15, App->frisbee->parabolaFinalY+15, &punterParabolaP);
	}

	//P1 UI
	App->render->Blit(uiSpriteTexture, 10, 10, &p1Rect);
	switch (App->sceneCharacterSelect->p1Char)
	{
	case Mita:
		App->render->Blit(uiSpriteTexture, 40, 10, &japanFlagRect);
		break;
	case Yoo:
		App->render->Blit(uiSpriteTexture, 40, 10, &koreanFlagRect);
		break;
	case Wessel:
		App->render->Blit(uiSpriteTexture, 40, 10, &germanyFlagRect);
		break;
	}

	//P2 UI
	App->render->Blit(uiSpriteTexture, 200, 10, &p2Rect);
	switch (App->sceneCharacterSelect->p2Char)
	{
	case Mita:
		App->render->Blit(uiSpriteTexture, 230, 10, &japanFlagRect);
		break;
	case Yoo:
		App->render->Blit(uiSpriteTexture, 230, 10, &koreanFlagRect);
		break;
	case Wessel:
		App->render->Blit(uiSpriteTexture, 230, 10, &germanyFlagRect);
		break;
	}



	//Mostra qui ha guanyat i qui ha perdut al final de la partida
	//estadoS nomes depen de sceneBeach, cal canviar
	if (App->sceneBeachStage->estadoS == App->sceneBeachStage->FINAL)
	{
		if (App->sceneBeachStage->winState == 1) {

			App->render->Blit(leftCharTxt, 22, 78, NULL);
			App->render->Blit(rightCharLostTxt, 156, 78, NULL);
			App->render->Blit(uiSpriteTexture, 18, 48, &winUILeft);
			App->render->Blit(uiSpriteTexture, 175, 54, &LoseUIRight);
			ShowSetCount(!ambQuadradet);
			//winState = 4;

		}
		else if (App->sceneBeachStage->winState == 2) {

			App->render->Blit(leftCharLostTxt, 22, 78, NULL);
			App->render->Blit(rightCharTxt, 156, 78, NULL);
			App->render->Blit(uiSpriteTexture, 176, 48, &winUIRight);
			App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
			ShowSetCount(!ambQuadradet);
			//winState = 4;

		}
		else if (App->sceneBeachStage->winState == 3) {

			App->render->Blit(leftCharLostTxt, 24, 78, NULL);
			App->render->Blit(rightCharLostTxt, 156, 78, NULL);
			App->render->Blit(uiSpriteTexture, 46, 36, &drawGameRect);
			ShowSetCount(!ambQuadradet);
			//App->render->Blit(uiSpriteTexture, 18, 48, &LoseUIRight);
			//App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
			//winState = 4;
		}
	}

	// Rectangulets. S'encen si a la seguent ronda el player guanyara
	if (App->player->round > App->player2->round)
	{
		App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
	}
	else if (App->player->round < App->player2->round)
	{
		App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
	}
	else if (App->player->round == App->player2->round && App->player->round >= 1 && App->player2->round >= 1)
	{
		App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
		App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
	}

	App->render->Blit(uiSpriteTexture, 150, 150, &rounds);

	//INICI PARTIDA
	if (App->sceneBeachStage->estadoS == App->sceneBeachStage->INICIO)
	{
		//12 points / 30 sec
		App->render->Blit(uiSpriteTexture, 77, 168, &rectNormes);

		//Diferents puntuacions
		if (App->sceneStageSelect->sceneSelected == Concrete)
		{
			//puntuaci� tots gols L
			App->render->Blit(uiSpriteTexture, 12, 26, &cincPuntsL);
			App->render->Blit(uiSpriteTexture, 12, 92, &tresPuntsL);
			App->render->Blit(uiSpriteTexture, 12, 158, &cincPuntsL);

			//puntuaci� tots gols R
			App->render->Blit(uiSpriteTexture, 232, 26, &cincPuntsR);
			App->render->Blit(uiSpriteTexture, 232, 92, &tresPuntsR);
			App->render->Blit(uiSpriteTexture, 232, 158, &cincPuntsR);
		}
		else
		{
			//puntuaci� tots gols L
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);

			//puntuaci� tots gols R
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);
		}

		//Set1
		App->render->Blit(uiSpriteTexture, 72, 80, &set1Rect);

		//Time88
		App->render->Blit(timerTexture, 144, 13, &rectTimer88);

		//Score 00-00 preGame
		App->fonts->BlitText(114, 17, App->player->scoreFont, "0");
		App->fonts->BlitText(130, 17, App->player->scoreFont, "0");
		App->fonts->BlitText(162, 17, App->player->scoreFont, "0");
		App->fonts->BlitText(178, 17, App->player->scoreFont, "0");

	}
	//COMEN�A PARTIDA
	else if (App->sceneBeachStage->estadoS == App->sceneBeachStage->RONDA)
	{

		//// Draw UI P1(score) --------------------------------------
		//if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
		//{
		//	sprintf_s(scoreText, 10, "%2d", App->player->score);

		//	App->fonts->BlitText(117, 17, scoreFont, scoreText);

		//	//App->fonts->BlitText(20, 150, scoreFont, "0 1 2 3 4 5 6 7 8 9 G");
		//}
		//// Draw UI P2(score2) --------------------------------------
		//if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
		//{
		//	sprintf_s(scoreText2, 10, "%2d", App->player2->score);

		//	App->fonts->BlitText(165, 17, scoreFont2, scoreText2);
		//}

		//Timer
		//rectTimer = currentTimerAnim->GetCurrentFrame();
		//App->render->Blit(timerTexture, 144, 13, &rectTimer);
	}

	rectTimer = currentTimerAnim->GetCurrentFrame();
	App->render->Blit(timerTexture, 144, 13, &rectTimer);
	// Draw UI P1(score) --------------------------------------
	if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
	{
		sprintf_s(scoreText, 10, "%2d", App->player->score);

		App->fonts->BlitText(117, 17, scoreFont, scoreText);

		//App->fonts->BlitText(20, 150, scoreFont, "0 1 2 3 4 5 6 7 8 9 G");
	}
	// Draw UI P2(score2) --------------------------------------
	if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
	{
		sprintf_s(scoreText2, 10, "%2d", App->player2->score);

		App->fonts->BlitText(165, 17, scoreFont2, scoreText2);
	}

	//---------------------------------------------------------------------------------------------------------

	//ENTRE RONDES BLIT DE QUINA RONDA ES
	if (App->sceneBeachStage->roundSpriteAppear == true)
	{
		//1 a 0 __ 0 a 1 SET 2
		if ((App->player->round == 1 && App->player2->round == 0) || (App->player->round == 0 && App->player2->round == 1))
		{
			App->render->Blit(uiSpriteTexture, 72, 80, &set2Rect);
		}
		//1 a 2 __ 2 a 1 __ 1 a 1 SET FINAL
		else if ((App->player->round == 1 && App->player2->round == 2) || (App->player->round == 2 && App->player2->round == 1)
			|| (App->player->round == 1 && App->player2->round == 1))
		{
			App->render->Blit(uiSpriteTexture, 72, 80, &setFinalRect);
		}
		//2 a 2 SET SUDDENDEATH
		else if ((App->player->round == 2 && App->player2->round == 2))
		{
			App->render->Blit(uiSpriteTexture, 17, 94, &suddenRect);
		}
		//3 a 3 DRAW GAME
		else if ((App->player->round == 3 && App->player2->round == 3))
		{
			//DRAW GAME
		}
	}

	//Entre Rondes indicar quants sets porta cadasc�
	if (App->sceneBeachStage->setsSpriteAppear == true)
	{
		ShowSetCount(ambQuadradet);
	}


	if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
	{
		if (!isDebugAppear)
			isDebugAppear = true;
		else isDebugAppear = false;
	}

	if (isDebugAppear)
	{
		// Despres de qualsevol blit perque estigui per sobre de tot
		// En "", posar la variable que es vulgui imprimir per pantalla (scoreExemple)
		// Si no es fan servir variables, comentar aquesta linia

		sprintf_s(debugText, 10, "%2d", App->sceneBeachStage->initialTimeS);

		// A "TEST TEXT", escriure el que es vulgui: una string (igual que l'exempel) o la variable debugText,
		// que correspon a la variable que s'hagi posat al quart parametre de sprintf_s, "".

		App->fonts->BlitText(72, 190, debugFont, debugText);
	}

	//Depenent de quin stage is selected (punts gol posicions)
	if (App->sceneStageSelect->sceneSelected == Beach)
	{
		/*
		UPLEFT,
		MIDLEFT,
		DOWNLEFT,
		UPRIGHT,
		MIDRIGHT,
		DOWNRIGHT,
		MISSL,
		MISSR,
		*/
		switch (App->sceneBeachStage->estadoGolScore)
		{
		case (App->sceneBeachStage->UPLEFT):
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			break;
		case(App->sceneBeachStage->MIDLEFT):
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			break;
		case(App->sceneBeachStage->DOWNLEFT):
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);
			break;
		case(App->sceneBeachStage->UPRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			break;
		case(App->sceneBeachStage->MIDRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			break;
		case(App->sceneBeachStage->DOWNRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);
			break;
		case(App->sceneBeachStage->MISSL):
			App->render->Blit(uiSpriteTexture, posXFrisbeeMiss, posYFrisbeeMiss, &dosPunts); //72- 58
			break;
		case(App->sceneBeachStage->MISSR):
			App->render->Blit(uiSpriteTexture, posXFrisbeeMiss, posYFrisbeeMiss, &dosPunts); //192-58
			break;
		}
	}
	else if (App->sceneStageSelect->sceneSelected == Lawn)
	{
		switch (App->sceneBeachStage->estadoGolScore)
		{
		case (App->sceneBeachStage->UPLEFT):
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			break;
		case(App->sceneBeachStage->MIDLEFT):
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			break;
		case(App->sceneBeachStage->DOWNLEFT):
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);
			break;
		case(App->sceneBeachStage->UPRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			break;
		case(App->sceneBeachStage->MIDRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			break;
		case(App->sceneBeachStage->DOWNRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);
			break;
		case(App->sceneBeachStage->MISSL):
			App->render->Blit(uiSpriteTexture, posXFrisbeeMiss, posYFrisbeeMiss, &dosPunts); //72-58
			break;
		case(App->sceneBeachStage->MISSR):
			App->render->Blit(uiSpriteTexture, posXFrisbeeMiss, posYFrisbeeMiss, &dosPunts); //192-58
			break;
		}
	}
	else if (App->sceneStageSelect->sceneSelected == Concrete)
	{
		switch (App->sceneBeachStage->estadoGolScore)
		{
		case (App->sceneBeachStage->UPLEFT):
			App->render->Blit(uiSpriteTexture, 7, 30, &cincPuntsL);
			break;
		case(App->sceneBeachStage->MIDLEFT):
			App->render->Blit(uiSpriteTexture, 7, 92, &tresPuntsL);
			break;
		case(App->sceneBeachStage->DOWNLEFT):
			App->render->Blit(uiSpriteTexture, 7, 158, &cincPuntsL);
			break;
		case(App->sceneBeachStage->UPRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 30, &cincPuntsR);
			break;
		case(App->sceneBeachStage->MIDRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 92, &tresPuntsR);
			break;
		case(App->sceneBeachStage->DOWNRIGHT):
			App->render->Blit(uiSpriteTexture, 236, 158, &cincPuntsR);
			break;
		case(App->sceneBeachStage->MISSL):
			App->render->Blit(uiSpriteTexture, posXFrisbeeMiss, posYFrisbeeMiss, &dosPunts); //72-58
			break;
		case(App->sceneBeachStage->MISSR):
			App->render->Blit(uiSpriteTexture, posXFrisbeeMiss, posYFrisbeeMiss, &dosPunts);//192-58
			break;
		}
	}


	if (isDebugAppear)
	{
		if (App->sceneBeachStage->estadoTGol == App->sceneBeachStage->FINGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "FINGOL");
		}
		if (App->sceneBeachStage->estadoTGol == App->sceneBeachStage->EJECUTANDOGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "EJECUTANDOGOL");
		}
		if (App->sceneBeachStage->estadoTGol == App->sceneBeachStage->INICIOGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "INICIOGOL");
		}
		sprintf_s(debugText, 10, "%2d", App->sceneBeachStage->timeLimitGol);
		App->fonts->BlitText(72, 210, debugFont, debugText);
		sprintf_s(debugText, 10, "%2d", App->sceneBeachStage->currentTimeGol);
		App->fonts->BlitText(112, 210, debugFont, debugText);
	}


	return Update_Status::UPDATE_CONTINUE;
}



bool ModuleInGameUI::CleanUp()
{
	App->textures->Unload(uiSpriteTexture);
	App->textures->Unload(timerTexture);
	App->textures->Unload(rectanguletLila);
	App->textures->Unload(leftCharTxt);
	App->textures->Unload(rightCharTxt);
	App->textures->Unload(vsTextureTxt);
	App->textures->Unload(leftNameTxt);
	App->textures->Unload(rightNameTxt);
	App->textures->Unload(punterParabola);
	App->fonts->UnLoad(debugFont);
	App->fonts->UnLoad(scoreFont);
	App->fonts->UnLoad(scoreFont2);
	App->textures->Unload(leftCharLostTxt);
	App->textures->Unload(rightCharLostTxt);


	return true;
}

void ModuleInGameUI::TimerS() {
	currentTimeS = SDL_GetTicks();

	if (currentTimeS - initialTimeS >= timeLimitS) {
		estadoTS = estadoTimerS::FIN;
	}
}

void ModuleInGameUI::ShowSetCount(bool ambQuadradet)
{
	if (ambQuadradet)
	{
		App->render->Blit(rectanguletLila, 64, 89, NULL);
		verticalPos = 107;
	}
	else
	{
		verticalPos = 157;
	}

	App->render->Blit(uiSpriteTexture, 113, verticalPos, &setCountRect);

	//Num of sets P1
	if (App->player->round == 0)
	{
		App->render->Blit(uiSpriteTexture, 50, verticalPos, &setNum0);
	}
	else if (App->player->round == 1)
	{
		App->render->Blit(uiSpriteTexture, 50, verticalPos, &setNum1);
	}
	else if (App->player->round == 2)
	{
		App->render->Blit(uiSpriteTexture, 50, verticalPos, &setNum2);
	}
	else if (App->player->round == 3)
	{
		App->render->Blit(uiSpriteTexture, 50, verticalPos, &setNum3);
	}

	//Num of sets P2
	if (App->player2->round == 0)
	{
		App->render->Blit(uiSpriteTexture, 210, verticalPos, &setNum0);
	}
	else if (App->player2->round == 1)
	{
		App->render->Blit(uiSpriteTexture, 210, verticalPos, &setNum1);
	}
	else if (App->player2->round == 2)
	{
		App->render->Blit(uiSpriteTexture, 210, verticalPos, &setNum2);
	}
	else if (App->player2->round == 3)
	{
		App->render->Blit(uiSpriteTexture, 210, verticalPos, &setNum3);
	}
}