#include "SceneCharacterSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleParticles.h"
#include "SceneBeachStage.h"
#include "SceneTitle.h"


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
	if (App->sceneTitle->loadFxOnceTitle == 0) {
		moveFx = 0;
		hiromiSelected = 0;
		yooSelected = 0;
		wesselSelected = 0;
		hiromiSelected = App->audio->LoadFx("Assets/Fx/HiromiSelected.wav");
		yooSelected = App->audio->LoadFx("Assets/Fx/B.YooSelect.wav");
		wesselSelected = App->audio->LoadFx("Assets/Fx/KlaussSelected.wav");
		moveFx = App->audio->LoadFx("Assets/FX/MoveMenu.wav");
	}

	

	LOG("Loading background assets");

	bool ret = true;

	//P1
	x1 = 9;
	y1 = 72;
	p1Char = Mita;
	p1Selected = false;

	//P2
	x2 = 33;
	y2 = 72;
	p2Char = Mita;
	p2Selected = false;

	bgTexture = App->textures->Load("Assets/Sprites/UI/SelectPlayer.png");
	uiSpriteSheet = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");

	App->audio->PlayMusic("Assets/Music/01_Get Ready (Select Screen).ogg", 0.0f);
	

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

bool SceneCharacterSelect::CleanUp() {
	App->textures->Unload(bgTexture);
	App->textures->Unload(uiSpriteSheet);
	return true;
}

Update_Status SceneCharacterSelect::Update()
{
	//Input per P1
	if (!p1Selected)
	{
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

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
		{
			if (x1 == 9 && y1 == 72)
			{
				App->audio->PlayFx(hiromiSelected);
				p1Char = Mita;
				p1Selected = true;
			}
			else if (x1 == 152 && y1 == 72)
			{
				App->audio->PlayFx(yooSelected);
				p1Char = Yoo;
				p1Selected = true;
			}
			else if (x1 == 152 && y1 == 184)
			{
				App->audio->PlayFx(wesselSelected);
				p1Char = Wessel;
				p1Selected = true;
			}
		}
	}


	//Input per P2
	if (!p2Selected)
	{
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

		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_DOWN)
		{
			if (x2 == 177)
			{
				x2 = 33;
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

		//SPACE PASSA A SER RETURN (ENTER)
		if (App->input->keys[SDL_SCANCODE_RETURN] == Key_State::KEY_DOWN)
		{
			if (x2 == 33 && y2 == 72)
			{
				App->audio->PlayFx(hiromiSelected);
				p2Char = Mita;
				p2Selected = true;
			}
			else if (x2 == 177 && y2 == 72)
			{
				App->audio->PlayFx(yooSelected);
				p2Char = Yoo;
				p2Selected = true;
			}
			else if (x2 == 177 && y2 == 184)
			{
				App->audio->PlayFx(wesselSelected);
				p2Char = Wessel;
				p2Selected = true;
			}
		}
	}

	//Transicio nomes quan els dos han seleccionat
	if (p1Selected && p2Selected)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneStageSelect, 30);
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

	if (App->input->keys[SDL_SCANCODE_F6] == Key_State::KEY_DOWN)
	{
		App->particles->AddParticle(0, 0, App->particles->mitaLoadShotParticle, 110, 120, Collider::NONE, 0);
	}

	return Update_Status::UPDATE_CONTINUE;
}

