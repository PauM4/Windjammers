#include "ModulePlayer2.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include <stdio.h>

ModulePlayer2::ModulePlayer2(bool startEnabled) : Module(startEnabled)
{
	// idle right animation
	idleAnim.PushBack({ 393, 103, 23, 36 });
	idleAnim.PushBack({ 370, 103, 23, 35 });
	idleAnim.PushBack({ 347, 103, 23, 36 });
	idleAnim.PushBack({ 323, 103, 24, 36 });
	idleAnim.PushBack({ 299, 103, 24, 35 });
	idleAnim.PushBack({ 275, 103, 24, 36 });
	idleAnim.PushBack({ 251, 103, 23, 39 });
	idleAnim.PushBack({ 227, 103, 23, 37 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	// move right
	rightAnim.PushBack({ 454, 148, 45, 32 });
	rightAnim.PushBack({ 426, 148, 28, 33 });
	rightAnim.PushBack({ 385, 148, 41, 36 });
	rightAnim.PushBack({ 339, 148, 46,	29 });
	rightAnim.PushBack({ 310, 148, 29, 36 });
	rightAnim.PushBack({ 269, 148, 41, 35 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;

	// Move down
	downAnim.PushBack({ 240, 148, 29, 43 });
	downAnim.PushBack({ 208, 148, 32, 38 });
	downAnim.PushBack({ 179, 148, 29, 35 });
	downAnim.PushBack({ 155, 148, 24, 41 });
	downAnim.PushBack({ 125, 148, 30, 38 });
	downAnim.PushBack({ 94, 148, 31, 34 });
	downAnim.loop = true;
	downAnim.speed = 0.1f;

	//Move Up
	upAnim.PushBack({ 150, 191, 30, 40 });
	upAnim.PushBack({ 120, 191, 30, 51 });
	upAnim.PushBack({ 90, 191, 30, 50 });
	upAnim.PushBack({ 60, 191, 30, 40 });
	upAnim.PushBack({ 30, 191, 30, 53 });
	upAnim.PushBack({ 0, 191, 30, 50 });
	upAnim.loop = true;
	upAnim.speed = 0.1f;

	//Move Left
	leftAnim.PushBack({ 455, 404, 45, 32 });
	leftAnim.PushBack({ 427, 404, 28, 33 });
	leftAnim.PushBack({ 386, 404, 40, 36 });
	leftAnim.PushBack({ 340, 404, 45, 29 });
	leftAnim.PushBack({ 311, 404, 28, 36 });
	leftAnim.PushBack({ 270, 404, 40, 35 });
	leftAnim.loop = true;
	leftAnim.speed = 0.1f;

}

ModulePlayer2::~ModulePlayer2()
{

}

bool ModulePlayer2::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Characters/Jap.png");
	currentAnimation = &idleAnim;


	position.x = 264;
	position.y = 100;

	// TODO 4: Retrieve the player when playing a second time
	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 27, 31 }, Collider::Type::PLAYER, this);

	return ret;
}

Update_Status ModulePlayer2::Update()
{
	// Moving the player with the camera scroll
	//App->player->position.x += 1;

	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT && position.x > 155)
	{
		position.x -= speed;

		if (currentAnimation != &leftAnim && App->input->keys[SDL_SCANCODE_UP] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_DOWN] != Key_State::KEY_REPEAT)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT && position.x < 254)
	{
		position.x += speed;
		if (currentAnimation != &rightAnim && App->input->keys[SDL_SCANCODE_UP] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_DOWN] != Key_State::KEY_REPEAT)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT && position.y < 150)
	{
		position.y += speed;
		if (currentAnimation != &downAnim)
		{
			downAnim.Reset();
			currentAnimation = &downAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT && position.y > 50)
	{
		position.y -= speed;
		if (currentAnimation != &upAnim)
		{
			upAnim.Reset();
			currentAnimation = &upAnim;
		}
	}

	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_K] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{
			App->particles->AddParticle(-5, -5, App->particles->frisbee, position.x - 20, position.y - 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}


		if (App->input->keys[SDL_SCANCODE_K] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			App->particles->AddParticle(-5, 5, App->particles->frisbee, position.x - 20, position.y + 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;

		}

		if (App->input->keys[SDL_SCANCODE_K] == Key_State::KEY_DOWN)
		{
			App->particles->AddParticle(-5, 0, App->particles->frisbee, position.x - 20, position.y, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}
	}

	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_J] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{
			App->particles->AddParticle(-2.5, -1, App->particles->frisbeeProjectile, position.x - 20, position.y - 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}


		if (App->input->keys[SDL_SCANCODE_J] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			App->particles->AddParticle(-2.5, 1, App->particles->frisbeeProjectile, position.x - 20, position.y + 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;

		}

		if (App->input->keys[SDL_SCANCODE_J] == Key_State::KEY_DOWN)
		{
			App->particles->AddParticle(-2.5, 0, App->particles->frisbeeProjectile, position.x - 20, position.y, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}
	}




	collider->SetPos(position.x, position.y);

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE)
		currentAnimation = &idleAnim;

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer2::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer2::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{//le pongo 0,0 pq no se exactamente q es esto y como he cambiado la funcion como tal tengo q meterle estos parametros i o si
		App->particles->AddParticle(0, 0, App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		App->particles->AddParticle(0, 0, App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
		App->particles->AddParticle(0, 0, App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
		App->particles->AddParticle(0, 0, App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
		App->particles->AddParticle(0, 0, App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

		App->audio->PlayFx(explosionFx);

		//TODO 3: Go back to the intro scene when the player gets killed
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneIntro, 60);

		destroyed = true;
	}
}