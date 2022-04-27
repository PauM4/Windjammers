#include "SceneStageSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

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
	LOG("Loading background assets");

	bool ret = true;

	//Beach only available
	MapList::Beach;
	yMove = 55;
	x1 = 24;
	// -200 outside screen
	x2 = -200;
	x3 = -200;
	x4 = -200;
	x5 = -200;
	x6 = -200;

	bgStageTexture = App->textures->Load("Assets/Sprites/UI/stageSelectBg.png");
	selectSquareTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");
	stageNamesTexture = App->textures->Load("Assets/Sprites/UI/stageNames.png");

	miniClayTexture = App->textures->Load("Assets/Sprites/UI/miniClay.png");
	miniConcreteTexture = App->textures->Load("Assets/Sprites/UI/miniConcrete.png");
	miniLawnTexture = App->textures->Load("Assets/Sprites/UI/miniLawn.png");
	miniStadiumTexture = App->textures->Load("Assets/Sprites/UI/miniStadium.png");
	miniTiledTexture = App->textures->Load("Assets/Sprites/UI/miniTiled.png");
	miniBeachTexture = App->textures->Load("Assets/Sprites/UI/miniBeach.png");

	selectFx = App->audio->LoadFx("Assets/FX/Select.wav");
	moveFx = App->audio->LoadFx("Assets/FX/MoveMenu.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;		

	return ret;
}

Update_Status SceneStageSelect::Update()
{
	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_DOWN)
	{
		if (yMove == 55)
		{
			MapList::Beach;
			yMove = 79;
			x1 = -200;
			x2 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 79)
		{
			MapList::Lawn;
			yMove = 103;
			x2 = -200;
			x3 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 103)
		{
			MapList::Tiled;
			yMove = 127;
			x3 = -200;
			x4 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 127)
		{
			MapList::Concrete;
			yMove = 151;
			x4 = -200;
			x5 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 151)
		{
			MapList::Clay;
			yMove = 175;
			x5 = -200;
			x6 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 175)
		{
			MapList::Stadium;
			yMove = 175;
			x6 = 24;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_DOWN)
	{
		if (yMove == 79)
		{
			MapList::Lawn;
			yMove = 55;
			x2 = -200;
			x1 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 103)
		{
			MapList::Tiled;
			yMove = 79;
			x3 = -200;
			x2 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 127)
		{
			MapList::Concrete;
			yMove = 103;
			x4 = -200;
			x3 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 151)
		{
			MapList::Clay;
			yMove = 127;
			x5 = -200;
			x4 = 24;
			App->audio->PlayFx(moveFx);
		}
		else if (yMove == 175)
		{
			MapList::Stadium;
			yMove = 151;
			x6 = -200;
			x5 = 24;
			App->audio->PlayFx(moveFx);
		}
		//else if (yMove == 55)
		//{
		//	MapList::Stadium;
		//	yMove = 175;
		//	x3 = -200;
		//	x2 = 24;
		//	App->audio->PlayFx(moveFx);
		//}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		if (yMove == 55)
		{
			App->audio->PlayFx(selectFx);
			App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
		}
		////QUAN CREEM NOUS ESCENARIS; CANVIAR AIXÒ
		//else if (yMove == 79)
		//{
		//	App->audio->PlayFx(selectFx);
		//	App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
		//}
		//else if (yMove == 104)
		//{
		//	App->audio->PlayFx(selectFx);
		//	App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
		//}
	}




	//if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	//{		
	//	App->audio->PlayFx(selectFx);
	//	App->fade->FadeToBlack(this, (Module*)App->sceneCharacterPresent, 15);
	//}

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
	App->render->Blit(miniTiledTexture, x3, 84, NULL);
	App->render->Blit(miniConcreteTexture, x4, 84, NULL);
	App->render->Blit(miniClayTexture, x5, 84, NULL);
	App->render->Blit(miniStadiumTexture, x6, 84, NULL);


	return Update_Status::UPDATE_CONTINUE;
}