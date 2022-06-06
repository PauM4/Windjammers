#include <stdio.h>

#include "SceneNeogeo.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

#include "SDL/include/SDL.h"

SceneNeogeo::SceneNeogeo(bool startEnabled) : Module(startEnabled)
{
	
}

SceneNeogeo::~SceneNeogeo()
{

}

// Load assets
bool SceneNeogeo::Start()
{
	//selectFx = 0;
	LOG("Loading background assets");

	bool ret = true;
	char x[128];

	for (int i = 0; i < NUM_IMAGES; i++) {
		sprintf_s(x, "Assets/Sprites/UI/Neogeo/neogeo%d.png", i + 1);
		bgTexture[i] = App->textures->Load(x);
	}
	frame = 0;
	timer = 0;

	////SILENT AUDIO per aturar la música de IntroScreen
	App->audio->PlayMusic("Assets/Music/introTitle.ogg", 0.0f);
	////selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

bool SceneNeogeo::CleanUp() {
	for (int i = 0; i < NUM_IMAGES; i++) {
		App->textures->Unload(bgTexture[i]);
	}
	return true;
}

Update_Status SceneNeogeo::Update()
{
	if (timer == 2) {
		if (frame < NUM_IMAGES - 1) {
			frame++;
			timer = 0;
		}
	}
	timer++;

	if (next < 360)
	{
		next++;
	}
	else if (next == 360)
	{
		scape = true;
	}

	if (scape)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneDataeast, 30);
	}
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		//App->audio->PlayFx(selectFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneDataeast, 30);
	}

	//currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneNeogeo::PostUpdate()
{
	// Draw everything --------------------------------------
	//SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture[frame], 0, 0, NULL);
	//App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}