#include "SceneCharacterSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneCharacterSelect::SceneCharacterSelect(bool startEnabled) : Module(startEnabled)
{
	p1Rect = { 359, 11, 20, 16 };
	p2Rect = { 392, 11, 23, 16 };
}

SceneCharacterSelect::~SceneCharacterSelect()
{

}

// Load assets
bool SceneCharacterSelect::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	//P1
	CharList::Mita;
	x1 = 9;
	y1 = 72;
	
	//P2
	x2 = 33;
	y2 = 72;


	bgTexture = App->textures->Load("Assets/Sprites/UI/SelectPlayer.png");
	uiSpriteSheet = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");

	App->audio->PlayMusic("Assets/Music/01_Get Ready (Select Screen).ogg", 0.0f);
	hiromiSelected = App->audio->LoadFx("Assets/Fx/HiromiSelected.wav");
	moveFx = App->audio->LoadFx("Assets/FX/MoveMenu.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneCharacterSelect::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN && x1 == 9 && y1 == 72
		&& x2 == 33 && y2 == 72)
	{
		App->audio->PlayFx(hiromiSelected);
		App->fade->FadeToBlack(this, (Module*)App->sceneStageSelect, 30);
	}

	//Input per P1
	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_DOWN)
	{
		if (y1 == 184)
		{
			y1 = 128;
			App->audio->PlayFx(moveFx);
		}
		else if (y1 == 128)
		{
			y1 = 72;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_DOWN)
	{
		if (x1 == 152)
		{
			x1 = 9;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_DOWN)
	{
		if (y1 == 72)
		{
			y1 = 128;
			App->audio->PlayFx(moveFx);
		}
		else if (y1 == 128)
		{
			y1 = 184;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_DOWN)
	{
		if (x1 == 9)
		{
			x1 = 152;
			App->audio->PlayFx(moveFx);
		}
	}

	//Input per P2
	if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_DOWN)
	{
		if (y2 == 184)
		{
			y2 = 128;
			App->audio->PlayFx(moveFx);
		}
		else if (y2 == 128)
		{
			y2 = 72;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
	{
		if (x2 == 177)
		{
			x2 = 33;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_DOWN)
	{
		if (y2 == 72)
		{
			y2 = 128;
			App->audio->PlayFx(moveFx);
		}
		else if (y2 == 128)
		{
			y2 = 184;
			App->audio->PlayFx(moveFx);
		}
	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_DOWN)
	{
		if (x2 == 33)
		{
			x2 = 177;
			App->audio->PlayFx(moveFx);
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneCharacterSelect::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(uiSpriteSheet, x1, y1, &p1Rect);
	App->render->Blit(uiSpriteSheet, x2, y2, &p2Rect);

	return Update_Status::UPDATE_CONTINUE;
}