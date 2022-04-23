#include "SceneTitle.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneTitle::SceneTitle(bool startEnabled) : Module(startEnabled)
{

}

SceneTitle::~SceneTitle()
{

}

// Load assets
bool SceneTitle::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/titleScreen.png");
	selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneTitle::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(selectFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneStageSelect, 30);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneTitle::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}