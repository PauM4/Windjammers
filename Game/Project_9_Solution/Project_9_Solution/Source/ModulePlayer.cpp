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
	downAnim.PushBack({ 393, 103, 23, 36 });
	downAnim.PushBack({ 370, 103, 23, 35 });
	downAnim.PushBack({ 347, 103, 23, 36 });
	downAnim.PushBack({ 323, 103, 24, 36 });
	downAnim.PushBack({ 299, 103, 24, 35 });
	downAnim.PushBack({ 275, 103, 24, 36 });
	downAnim.PushBack({ 251, 103, 23, 39 });
	downAnim.PushBack({ 227, 103, 23, 37 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;

	// move right
	upAnim.PushBack({ 130, 145, 135, 138 }); //2
	upAnim.PushBack({ 379, 144, 133, 132 }); //4
	upAnim.PushBack({ 263, 147, 118, 151 }); //3
	upAnim.PushBack({ 379, 144, 133, 132 }); //4
	upAnim.PushBack({ 130, 145, 135, 138 }); //2
	upAnim.PushBack({ 646, 145, 137, 137 }); // 5
	upAnim.PushBack({ 130, 144, 131, 132 }); // 1
	upAnim.PushBack({ 764, 147, 121, 150 }); //6
	upAnim.PushBack({ 130, 144, 131, 132 }); // 1
	upAnim.PushBack({ 646, 145, 137, 137 }); // 5
	upAnim.loop = true;
	upAnim.speed = 0.1f;

	// Move down
	downAnim.PushBack({ 393, 103, 23, 36 });
	downAnim.PushBack({ 370, 103, 23, 35 });
	downAnim.PushBack({ 347, 103, 23, 36 });
	downAnim.PushBack({ 323, 103, 24, 36 });
	downAnim.PushBack({ 299, 103, 24, 35 });
	downAnim.PushBack({ 275, 103, 24, 36 });
	downAnim.PushBack({ 251, 103, 23, 39 });
	downAnim.PushBack({ 227, 103, 23, 37 });
	downAnim.loop = true;
	downAnim.speed = 0.1f;

	//Move Right
	downAnim.PushBack({ 393, 103, 23, 36 });
	downAnim.PushBack({ 370, 103, 23, 35 });
	downAnim.PushBack({ 347, 103, 23, 36 });
	downAnim.PushBack({ 323, 103, 24, 36 });
	downAnim.PushBack({ 299, 103, 24, 35 });
	downAnim.PushBack({ 275, 103, 24, 36 });
	downAnim.PushBack({ 251, 103, 23, 39 });
	downAnim.PushBack({ 227, 103, 23, 37 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;

	//Move Left
	leftAnim.PushBack({ 0, 1, 32, 14 });
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

	texture = App->textures->Load("Assets/Sprites/Jap.png");
	currentAnimation = &idleAnim;

	//laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	//explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 150;
	position.y = 120;

	// TODO 4: Retrieve the player when playing a second time
	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);

	return ret;
}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	//App->player->position.x += 1;

	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT)
	{
		position.x -= speed;
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT)
	{
		position.x += speed;
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)
	{
		position.y += speed;
		if (currentAnimation != &downAnim)
		{
			downAnim.Reset();
			currentAnimation = &downAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT)
	{
		position.y -= speed;
		if (currentAnimation != &upAnim)
		{
			upAnim.Reset();
			currentAnimation = &upAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
		App->audio->PlayFx(laserFx);
	}

	// If no up/down movement detected, set the current animation back to idle
	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE)
		currentAnimation = &idleAnim;

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
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

		App->audio->PlayFx(explosionFx);

		//TODO 3: Go back to the intro scene when the player gets killed
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneIntro, 60);

		destroyed = true;
	}
}