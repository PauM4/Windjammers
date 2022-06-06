#include <stdio.h>

#include "SceneTitle.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"


#include "SDL/include/SDL.h"

SceneTitle::SceneTitle(bool startEnabled) : Module(startEnabled)
{

}

SceneTitle::~SceneTitle()
{

}

// Load assets
bool SceneTitle::Start()
{
	if (loadFxOnceTitle == 0) {
		selectFx = 0;
		selectFx = App->audio->LoadFx("Assets/FX/Select.wav");
	}

	/*title.Reset();*/
	
	LOG("Loading background assets");

	bool ret = true;
	hasPlayed = false;

	char z[128];

	for (int i = 0; i < NUM_IMAGES; i++) {
		sprintf_s(z, "Assets/Sprites/UI/Title/title%d.png", i);
		bgTexture[i] = App->textures->Load(z);
	}
	frame = 0;
	timer = 0;

	//SILENT AUDIO per aturar la música de IntroScreen
	App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

bool SceneTitle::CleanUp() {
	for (int i = 0; i < NUM_IMAGES; i++) {
		App->textures->Unload(bgTexture[i]);
	}
	return true;
}

Update_Status SceneTitle::Update()
{
	if (timer == 2) {
		if (frame < NUM_IMAGES - 1) {
			frame++;
			timer = 0;
		}
	}
	timer++;
	
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		if (!hasPlayed)
		{
			App->audio->PlayFx(selectFx);
			hasPlayed = true;
		}
		
		App->fade->FadeToBlack(this, (Module*)App->sceneCharacterSelect, 30);
	}

	//currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneTitle::PostUpdate()
{
	// Draw everything --------------------------------------
	//SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture[frame], 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}