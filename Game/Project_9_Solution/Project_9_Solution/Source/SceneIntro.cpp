#include "SceneIntro.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{

}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	timer = 0;
	scape = false;
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/startScreen.png");
	

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

bool SceneIntro::CleanUp() {
	App->textures->Unload(bgTexture);
	return true;
}

Update_Status SceneIntro::Update()
{
	//10s
	if (timer < 600)
	{
		timer++;
	}
	else if (timer == 600)
	{
		scape = true;
	}


	if (scape)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneNeogeo, 30);
	}
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneNeogeo, 30);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}