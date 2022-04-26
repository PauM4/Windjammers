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
	bgEffectAnim.PushBack({ 0, 0, 304, 224 });
	//bgEffectAnim.PushBack({ 0, 448, 304, 224 });
	bgEffectAnim.PushBack({ 0, 224, 304, 224 });
	//bgEffectAnim.PushBack({ 0, 448, 304, 224 });
	bgEffectAnim.PushBack({ 0, 672, 304, 224 });
	//bgEffectAnim.PushBack({ 0, 448, 304, 224 });
	bgEffectAnim.loop = true;
	bgEffectAnim.pingpong = false;
	bgEffectAnim.speed = 0.8f;
}

SceneCharacterPresent::~SceneCharacterPresent()
{

}

// Load assets
bool SceneCharacterPresent::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/bgEffect_SpriteSheet.png");

	charactersPresentTexture = App->textures->Load("Assets/Sprites/UI/charactersPresent.png");
	App->audio->PlayMusic("Assets/Music/02_Go for Broke! (Round Start).ogg", 0.1f);
	//App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");

	currentAnimation = &bgEffectAnim;

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

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status SceneCharacterPresent::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(charactersPresentTexture, 0, 0, NULL);
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture, 0, 0, &rect);

	return Update_Status::UPDATE_CONTINUE;
}