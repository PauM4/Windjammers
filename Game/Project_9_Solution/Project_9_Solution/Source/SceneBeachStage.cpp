#include "SceneBeachStage.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"

#include "SDL/include/SDL.h"

SceneBeachStage::SceneBeachStage(bool startEnabled) : Module(startEnabled)
{

}

SceneBeachStage::~SceneBeachStage()
{

}

// Load assets
bool SceneBeachStage::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	//FX de rounds
	beachTexture = App->textures->Load("Assets/Sprites/Levels/PH_Beach.png");
	App->audio->PlayMusic("Assets/Music/03_Flying Power Disc (Beach Court).ogg", 1.0f);


	//Bottomside collider 
	// **********************ALTRES COLLIDERS AQUÍ; FER QUE NO EXPLOTI
	//App->collisions->AddCollider({ 0, 224, 3930, 16 }, Collider::Type::WALL);

	//First two columns colliders
/*	App->collisions->AddCollider({ 1375, 0, 111, 96 }, Collider::Type::WALL);
	App->collisions->AddCollider({ 1375, 145, 111, 96 }, Collider::Type::WALL);*/

	// Enemies ---
	//App->enemies->AddEnemy(Enemy_Type::REDBIRD, 600, 80);
	//App->enemies->AddEnemy(Enemy_Type::REDBIRD, 625, 80);
	//App->enemies->AddEnemy(Enemy_Type::REDBIRD, 640, 80);
	//App->enemies->AddEnemy(Enemy_Type::REDBIRD, 665, 80);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	//App->enemies->Enable();

	return ret;
}

Update_Status SceneBeachStage::Update()
{
	//App->render->camera.x += 3;

	// DEBUG INSTANT WIN
	if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN)
	{
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
		SDL_Delay(1500);
		App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}
	// DEBUG INSTANT LOSE
	if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN)
	{
		App->audio->PlayMusic("Assets/Music/09_Lost Set.ogg", 0.0f);
		SDL_Delay(500);
		App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneBeachStage::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(beachTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneBeachStage::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)

	return true;
}