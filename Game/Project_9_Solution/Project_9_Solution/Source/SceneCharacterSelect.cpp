#include "SceneCharacterSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneCharacterSelect::SceneCharacterSelect(bool startEnabled) : Module(startEnabled)
{

}

SceneCharacterSelect::~SceneCharacterSelect()
{

}

// Load assets
bool SceneCharacterSelect::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/characterSelectBackground.png");
	App->audio->PlayMusic("Assets/Music/01_Get Ready (Select Screen).ogg", 0.0f);
	hiromiSelected = App->audio->LoadFx("Assets/Fx/HiromiSelected.wav");
	selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneCharacterSelect::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(hiromiSelected);
		App->audio->PlayFx(selectFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneStageSelect, 30);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneCharacterSelect::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}