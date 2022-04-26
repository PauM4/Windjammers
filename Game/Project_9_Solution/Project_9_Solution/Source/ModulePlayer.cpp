#include "ModulePlayer.h"

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

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// idle right animation
	idleRAnim.PushBack({ 393, 103, 23, 36 });
	idleRAnim.PushBack({ 370, 103, 23, 35 });
	idleRAnim.PushBack({ 347, 103, 23, 36 });
	idleRAnim.PushBack({ 323, 103, 24, 36 });
	idleRAnim.PushBack({ 299, 103, 24, 35 });
	idleRAnim.PushBack({ 275, 103, 24, 36 });
	idleRAnim.PushBack({ 251, 103, 23, 39 });
	idleRAnim.PushBack({ 227, 103, 23, 37 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.1f;

	//idle left animation
	idleLAnim.PushBack({ 393, 359, 23, 36 });
	idleLAnim.PushBack({ 370, 359, 23, 35 });
	idleLAnim.PushBack({ 347, 359, 23, 36 });
	idleLAnim.PushBack({ 323, 359, 24, 36 });
	idleLAnim.PushBack({ 299, 359, 24, 35 });
	idleLAnim.PushBack({ 275, 359, 24, 36 });
	idleLAnim.PushBack({ 251, 359, 23, 39 });
	idleLAnim.PushBack({ 227, 359, 23, 37 });
	idleLAnim.loop = true;
	idleLAnim.speed = 0.1f;

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

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Characters/Jap.png");
	currentAnimation = &idleRAnim;


	position.x = 20;
	position.y = 100;

	// TODO 4: Retrieve the player when playing a second time
	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 27, 31 }, Collider::Type::PLAYER, this);

	return ret;
}

int last = 1; //Last Move

Update_Status ModulePlayer::Update()
{
	//MOVIMIENTO
	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT && position.x > 20)
	{
		position.x -= speed;

		if (currentAnimation != &leftAnim && App->input->keys[SDL_SCANCODE_W] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_S] != Key_State::KEY_REPEAT)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		last = 0;
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT && position.x < 110)
	{
		position.x += speed;
		if (currentAnimation != &rightAnim && App->input->keys[SDL_SCANCODE_W] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_S] != Key_State::KEY_REPEAT)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		last = 1;
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && position.y < 150)
	{
		position.y += speed;
		if (currentAnimation != &downAnim)
		{
			downAnim.Reset();
			currentAnimation = &downAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && position.y > 50)
	{
		position.y -= speed;
		if (currentAnimation != &upAnim)
		{
			upAnim.Reset();
			currentAnimation = &upAnim;
		}
	}

		if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE && last == 1)
		currentAnimation = &idleRAnim;

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE && last == 0)
		currentAnimation = &idleLAnim;
	
	//LANZAMIENTO DE DISCO NORMAL
	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_X] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && disco)
		{
			App->particles->AddParticle(5, -5, App->particles->frisbee, position.x + 20, position.y - 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}


		if (App->input->keys[SDL_SCANCODE_X] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && disco)
		{
			App->particles->AddParticle(5, 5, App->particles->frisbee, position.x + 20, position.y + 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;

		}

		if (App->input->keys[SDL_SCANCODE_X] == Key_State::KEY_DOWN && disco)
		{
			App->particles->AddParticle(5, 0, App->particles->frisbee, position.x + 20, position.y, Collider::Type::PLAYER_SHOT );
			App->audio->PlayFx(laserFx);

			break;
		}
	}

	//LANZAMIENTO DE DISCO PARÁBOLA
	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_Z] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && disco)
		{
			App->particles->AddParticle(2.5, -1, App->particles->frisbeeProjectile, position.x + 20, position.y - 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}


		if (App->input->keys[SDL_SCANCODE_Z] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && disco)
		{
			App->particles->AddParticle(2.5, 1, App->particles->frisbeeProjectile, position.x + 20, position.y + 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;

		}

		if (App->input->keys[SDL_SCANCODE_Z] == Key_State::KEY_DOWN && disco)
		{
			App->particles->AddParticle(2.5, 0, App->particles->frisbeeProjectile, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(laserFx);
			break;
		}
	}

	collider->SetPos(position.x, position.y);

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		//creo q si ya lo tenemos puesto en el disco que si choca no haga nada, no deberia hacer falta ponerlo aqui tmb
	}
}

void ModulePlayer::frisbeeCollision() {
	disco = true;
}