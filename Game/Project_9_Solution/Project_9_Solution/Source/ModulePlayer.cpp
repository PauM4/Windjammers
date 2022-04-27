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
#include "ModuleFrisbee.h"

#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// Idle Left Animation
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

	// Idle Right Animation
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

	// Move Right
	rightAnim.PushBack({ 454, 148, 45, 32 });
	rightAnim.PushBack({ 426, 148, 28, 33 });
	rightAnim.PushBack({ 385, 148, 41, 36 });
	rightAnim.PushBack({ 339, 148, 46,	29 });
	rightAnim.PushBack({ 310, 148, 29, 36 });
	rightAnim.PushBack({ 269, 148, 41, 35 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;

	// Move Down Head Left
	downLAnim.PushBack({ 240, 404, 29, 43 });
	downLAnim.PushBack({ 208, 404, 32, 38 });
	downLAnim.PushBack({ 179, 404, 29, 35 });
	downLAnim.PushBack({ 155, 404, 24, 41 });
	downLAnim.PushBack({ 125, 404, 30, 38 });
	downLAnim.PushBack({ 94, 404, 31, 34 });
	downLAnim.loop = true;
	downLAnim.speed = 0.1f;

	// Move Down Head Right
	downRAnim.PushBack({ 240, 148, 29, 43 });
	downRAnim.PushBack({ 208, 148, 32, 38 });
	downRAnim.PushBack({ 179, 148, 29, 35 });
	downRAnim.PushBack({ 155, 148, 24, 41 });
	downRAnim.PushBack({ 125, 148, 30, 38 });
	downRAnim.PushBack({ 94, 148, 31, 34 });
	downRAnim.loop = true;
	downRAnim.speed = 0.1f;

	//Move Up Head Left
	upLAnim.PushBack({ 150, 447, 30, 40 });
	upLAnim.PushBack({ 120, 447, 30, 51 });
	upLAnim.PushBack({ 90, 447, 30, 50 });
	upLAnim.PushBack({ 60, 447, 30, 40 });
	upLAnim.PushBack({ 30, 447, 30, 53 });
	upLAnim.PushBack({ 0, 447, 30, 50 });
	upLAnim.loop = true;
	upLAnim.speed = 0.1f;

	//Move Up Head Right
	upRAnim.PushBack({ 150, 191, 30, 40 });
	upRAnim.PushBack({ 120, 191, 30, 51 });
	upRAnim.PushBack({ 90, 191, 30, 50 });
	upRAnim.PushBack({ 60, 191, 30, 40 });
	upRAnim.PushBack({ 30, 191, 30, 53 });
	upRAnim.PushBack({ 0, 191, 30, 50 });
	upRAnim.loop = true;
	upRAnim.speed = 0.1f;

	//Move Left
	leftAnim.PushBack({ 455, 404, 45, 32 });
	leftAnim.PushBack({ 427, 404, 28, 33 });
	leftAnim.PushBack({ 386, 404, 40, 36 });
	leftAnim.PushBack({ 340, 404, 45, 29 });
	leftAnim.PushBack({ 311, 404, 28, 36 });
	leftAnim.PushBack({ 270, 404, 40, 35 });
	leftAnim.loop = true;
	leftAnim.speed = 0.1f;

	//Idle Disk
	idleDisk.PushBack({ 0, 512, 47, 48 });
	idleDisk.PushBack({ 48, 512, 47, 48 });
	idleDisk.PushBack({ 96, 512, 47, 48 });
	idleDisk.PushBack({ 144, 512, 47, 48 });
	idleDisk.PushBack({ 192, 512, 47, 48 });
	idleDisk.loop = true;
	idleDisk.speed = 0.1f;
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

	//SFX
	tossFx = App->audio->LoadFx("Assets/Fx/Toss.wav");
	lobFx = App->audio->LoadFx("Assets/Fx/Lob.wav");

	position.x = 20;
	position.y = 100;

	// TODO 4: Retrieve the player when playing a second time
	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 27, 31 }, Collider::Type::PLAYER, this);

	char lookupTable[] = { "0123456789G " };
	scoreFont = App->fonts->Load("Assets/Sprites/UI/Fonts/scoreFont.png", lookupTable, 1);

	return ret;
}

Update_Status ModulePlayer::Update()
{
	//MOVIMIENTO
	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT && position.x > 20 && !disco)
	{
		position.x -= speed;

		if (currentAnimation != &leftAnim && App->input->keys[SDL_SCANCODE_W] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_S] != Key_State::KEY_REPEAT)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		last1 = 0;
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT && position.x < 110 && !disco)
	{
		position.x += speed;
		if (currentAnimation != &rightAnim && App->input->keys[SDL_SCANCODE_W] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_S] != Key_State::KEY_REPEAT)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		last1 = 1;
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && position.y < 150 && !disco)
	{
		position.y += speed;
		if (currentAnimation != &downLAnim && last1 == 0)
		{
			downLAnim.Reset();
			currentAnimation = &downLAnim;
		}
		if (currentAnimation != &downRAnim && last1 == 1)
		{
			downLAnim.Reset();
			currentAnimation = &downRAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && position.y > 50 && !disco)
	{
		position.y -= speed;
		if (currentAnimation != &upLAnim && last1 == 0)
		{
			upLAnim.Reset();
			currentAnimation = &upLAnim;
		}
		if (currentAnimation != &upRAnim && last1 == 1)
		{
			upRAnim.Reset();
			currentAnimation = &upRAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE && last1 == 0 && !disco)
		currentAnimation = &idleLAnim;

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE && last1 == 1 && !disco)
		currentAnimation = &idleRAnim;

	//Al recibir disco hace idle con disco en la mano
	if (last1 != 2 && disco) {
		currentAnimation = &idleDisk;
	}

	//LANZAMIENTO DE DISCO NORMAL
	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_X] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && disco && App->frisbee->posesion == false)
		{
			App->frisbee->mov = 1;
			disco = false;
			App->frisbee->posesion = true;
			App->frisbee->currentAnimation2 = &App->frisbee->moving;
			App->audio->PlayFx(tossFx);
			break;
		}


		if (App->input->keys[SDL_SCANCODE_X] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && disco && App->frisbee->posesion == false)
		{
			App->frisbee->mov = 3;
			disco = false;
			App->frisbee->posesion = true;
			App->frisbee->currentAnimation2 = &App->frisbee->moving;
			App->audio->PlayFx(tossFx);
			break;

		}


		if (App->input->keys[SDL_SCANCODE_X] == Key_State::KEY_DOWN && disco && App->frisbee->posesion == false)
		{
			App->frisbee->mov = 2;
			disco = false;
			App->frisbee->posesion = true;
			App->frisbee->currentAnimation2 = &App->frisbee->moving;
			App->audio->PlayFx(tossFx);
			break;

		}

	}

	//LANZAMIENTO DE DISCO PARÁBOLA
	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_Z] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && disco)
		{
			App->particles->AddParticle(2.5, -1, App->particles->frisbeeProjectile, position.x + 20, position.y - 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(lobFx);
			break;
		}


		if (App->input->keys[SDL_SCANCODE_Z] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && disco)
		{
			App->particles->AddParticle(2.5, 1, App->particles->frisbeeProjectile, position.x + 20, position.y + 20, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(lobFx);
			break;

		}

		if (App->input->keys[SDL_SCANCODE_Z] == Key_State::KEY_DOWN && disco)
		{
			App->particles->AddParticle(2.5, 0, App->particles->frisbeeProjectile, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
			App->audio->PlayFx(lobFx);
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

	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%2d", score);
	

	// TODO 3: Blit the text of the score in at the bottom of the screen
	App->fonts->BlitText(115, 16, scoreFont, scoreText);

	//App->fonts->BlitText(20, 150, scoreFont, "0 1 2 3 4 5 6 7 8 9 G");

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		//creo q si ya lo tenemos puesto en el disco que si choca no haga nada, no deberia hacer falta ponerlo aqui tmb
		frisbeeCollision();

	}
}

void ModulePlayer::frisbeeCollision() {
	App->frisbee->mov = 0;
	disco = true;
	App->frisbee->xspeed = 3;
	App->frisbee->yspeed = 3;
	App->frisbee->position.x = position.x + 28;
	App->frisbee->position.y = position.y;
	App->frisbee->posesion = false;
}