#include "SceneCharacterPresent.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

#include "SDL/include/SDL.h"

SceneCharacterPresent::SceneCharacterPresent(bool startEnabled) : Module(startEnabled)
{

}

SceneCharacterPresent::~SceneCharacterPresent()
{

}

// Load assets
bool SceneCharacterPresent::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/bgEffect.png");
	charactersPresent = App->textures->Load("Assets/Sprites/UI/charactersPresent.png");
	App->audio->PlayMusic("Assets/Music/02_Go for Broke! (Round Start).ogg", 0.1f);
	//App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneCharacterPresent::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneBeachStage, 30);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
int hola = 0;
Update_Status SceneCharacterPresent::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(charactersPresent, 0, 0, NULL);
	//COSA RARA
	//switch (hola)
	//{
	//case 0:
	//	App->render->Blit(bgTexture, 0, 0, NULL);
	//	hola = 1;
	//	break;
	//case 1:
	//	App->render->Blit(charactersPresent, 0, 0, NULL);
	//	hola = 0;
	//	break;
	//default:
	//	break;
	//}

	return Update_Status::UPDATE_CONTINUE;
}