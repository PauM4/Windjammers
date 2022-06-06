#include "SceneStageSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"
#include "SceneCharacterSelect.h"
#include "SceneTitle.h"

#include <stdio.h>

SceneStageSelect::SceneStageSelect(bool startEnabled) : Module(startEnabled)
{
	selectSquare = { 15, 27, 120, 24 };
}

SceneStageSelect::~SceneStageSelect()
{

}

// Load assets
bool SceneStageSelect::Start()
{
	if (App->sceneTitle->loadFxOnceTitle == 0) {
		selectFx = 0;
		moveFx = 0;
		selectFx = App->audio->LoadFx("Assets/FX/Select.wav");
		moveFx = App->audio->LoadFx("Assets/FX/MoveMenu.wav");
	}

	
	LOG("Loading background assets");

	bool ret = true;

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	//Beach com a stage inicial
	sceneSelected = Beach;

	yMove = 55;
	x1 = 24;
	// -200 outside screen
	x2 = -1000;
	x3 = -1000;

	bgStageTexture = App->textures->Load("Assets/Sprites/UI/stageSelectBg.png");
	selectSquareTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");
	stageNamesTexture = App->textures->Load("Assets/Sprites/UI/stageNames.png");

	miniClayTexture = App->textures->Load("Assets/Sprites/UI/miniClay.png");
	miniConcreteTexture = App->textures->Load("Assets/Sprites/UI/miniConcrete.png");
	miniLawnTexture = App->textures->Load("Assets/Sprites/UI/miniLawn.png");
	miniStadiumTexture = App->textures->Load("Assets/Sprites/UI/miniStadium.png");
	miniTiledTexture = App->textures->Load("Assets/Sprites/UI/miniTiled.png");
	miniBeachTexture = App->textures->Load("Assets/Sprites/UI/miniBeach.png");

	

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

bool SceneStageSelect::CleanUp() {
	App->textures->Unload(bgStageTexture);
	App->textures->Unload(selectSquareTexture);
	App->textures->Unload(stageNamesTexture);
	App->textures->Unload(miniClayTexture);
	App->textures->Unload(miniConcreteTexture);
	App->textures->Unload(miniLawnTexture);
	App->textures->Unload(miniStadiumTexture);
	App->textures->Unload(miniTiledTexture);
	App->textures->Unload(miniBeachTexture);
	App->fonts->UnLoad(debugFont);
	return true;
}

Update_Status SceneStageSelect::Update()
{
	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_DOWN)
	{
		if (yMove == 55)
		{
			sceneSelected = Lawn;
			yMove = 79;
			x1 = -1000;
			x2 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 79)
		{
			sceneSelected = Concrete;
			yMove = 127;
			x2 = -1000;
			x3 = 24;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_DOWN)
	{
		if (yMove == 79)
		{
			sceneSelected = Beach;
			yMove = 55;
			x2 = -1000;
			x1 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 127)
		{
			sceneSelected = Lawn;
			yMove = 79;
			x3 = -1000;
			x2 = 24;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		if (yMove == 55)
		{
			App->audio->PlayFx(selectFx);
			App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
		}
		else if (yMove == 79)
		{
			App->audio->PlayFx(selectFx);
			App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
		}
		else if (yMove == 127)
		{
			App->audio->PlayFx(selectFx);
			App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
		}
	}

	if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
	{
		if (!isDebugAppear)
			isDebugAppear = true;
		else isDebugAppear = false;
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneStageSelect::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgStageTexture, 0, 0, NULL);
	App->render->Blit(selectSquareTexture, 164, yMove, &selectSquare);
	App->render->Blit(stageNamesTexture, 6, 1, NULL);

	App->render->Blit(miniBeachTexture, x1, 84, NULL);
	App->render->Blit(miniLawnTexture, x2, 84, NULL);
	//App->render->Blit(miniTiledTexture, x3, 84, NULL);
	App->render->Blit(miniConcreteTexture, x3, 84, NULL);
	//App->render->Blit(miniClayTexture, x5, 84, NULL);
	//App->render->Blit(miniStadiumTexture, x6, 84, NULL);

	if (isDebugAppear)
	{
		if (App->sceneCharacterSelect->p1Char == Mita)
		{
			App->fonts->BlitText(72, 190, debugFont, "P1 MITA");
		}
		else if (App->sceneCharacterSelect->p1Char == Yoo)
		{
			App->fonts->BlitText(72, 190, debugFont, "P1 YOO");
		}
		else if (App->sceneCharacterSelect->p1Char == Wessel)
		{
			App->fonts->BlitText(72, 190, debugFont, "P1 WESSEL");
		}

		if (App->sceneCharacterSelect->p2Char == Mita)
		{
			App->fonts->BlitText(72, 200, debugFont, "P2 MITA");
		}
		else if (App->sceneCharacterSelect->p2Char == Yoo)
		{
			App->fonts->BlitText(72, 200, debugFont, "P2 YOO");
		}
		else if (App->sceneCharacterSelect->p2Char == Wessel)
		{
			App->fonts->BlitText(72, 200, debugFont, "P2 WESSEL");
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

