#include "SceneStageSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneStageSelect::SceneStageSelect(bool startEnabled) : Module(startEnabled)
{

}

SceneStageSelect::~SceneStageSelect()
{

}

// Load assets
bool SceneStageSelect::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/beachStageSelect.png");
	App->audio->PlayMusic("Assets/Music/01_Get Ready (Select Screen).ogg", 1.0f);
	selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;		

	return ret;
}

Update_Status SceneStageSelect::Update()
{
	//TESTING 
	if (App->input->keys[SDL_SCANCODE_0] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(selectFx);
	}
	//---TESTING
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{		
		App->audio->PlayFx(selectFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 90);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneStageSelect::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}