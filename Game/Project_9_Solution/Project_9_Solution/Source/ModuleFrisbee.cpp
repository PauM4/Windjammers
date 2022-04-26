#include "ModuleFrisbee.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "SDL/include/SDL.h"

ModuleFrisbee::ModuleFrisbee(bool startEnabled) : Module(startEnabled)
{
	// moving animation
	moving.PushBack({ 117, 48, 16, 16 });
	moving.PushBack({ 149, 48, 16, 16 });
	moving.PushBack({ 181, 48, 16, 16 });
	moving.PushBack({ 213, 48, 16, 16 });
	moving.loop = true;
	moving.speed = 0.1f;

	// Projectile motion animation
	projectile.PushBack({ 35, 8, 32, 14 });
	projectile.PushBack({ 53, 7, 25, 31 });
	projectile.PushBack({ 79, 6, 36, 34 });
	projectile.PushBack({ 116, 5, 28, 36 });
	projectile.PushBack({ 145, 4, 21, 38 });
	projectile.PushBack({ 167, 3, 3, 39 });
	projectile.PushBack({ 171, 2, 23, 41 });
	projectile.PushBack({ 195, 2, 33, 42 });
	projectile.PushBack({ 229, 0, 43, 44 });
	projectile.loop = false;
	projectile.speed = 0.1f;

}

ModuleFrisbee::~ModuleFrisbee()
{

}

bool ModuleFrisbee::Start()
{
	currentAnimation2 = &moving;
	LOG("Loading frisbee textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Levels/Beach.png");


	position.x = 20;
	position.y = 100;


	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this);

	return ret;
}

Update_Status ModuleFrisbee::Update()
{
	//while (position.x != App->player->position.x && position.y != App->player->position.y) {
		/*uint delay = 1500;
		SDL_Delay(delay);*/

	if (arbitro == 1) {
		App->player->position.x = 20;
		App->player->position.y = 100;
		App->player2->position.x = 264;
		App->player2->position.y = 100;
		if (position.x != App->player->position.x || position.y != App->player->position.y) {
			position.x -= 3;
			position.y -= 2;
		}
	}
	if (arbitro == 2) {
		App->player->position.x = 20;
		App->player->position.y = 100;
		App->player2->position.x = 264;
		App->player2->position.y = 100;
		if (position.x != App->player2->position.x || position.y != App->player2->position.y) {
			position.x += 3;
			position.y -= 2;
		}
	}




	//MOV FRISBEE HACIA ARRIBA
	if (mov == 1 && position.x >= 19 && position.x <= 276) {

		if (pared == false && position.y >= 50) {

			position.x += xspeed;
			position.y -= yspeed;

		}
		else {
			pared = true;
		}

		if (pared == true && position.y < 170) {
			position.x += xspeed;
			position.y += yspeed;
		}
		else {
			pared = false;
		}

	}

	//MOV FRISBEE HORIZONTAL - PARA PLAYER2 *-1
	if (mov == 2 && position.x >= 19 && position.x <= 276) {
		position.x += xspeed;
	}

	//MOV FRISBEE HACIA ABAJO
	if (mov == 3 && position.x >= 19 && position.x <= 276) {

		if (pared == false && position.y < 170) {

			position.x += xspeed;
			position.y += yspeed;

		}
		else {
			pared = true;
		}

		if (pared == true && position.y >= 50) {
			position.x += xspeed;
			position.y -= yspeed;
		}
		else {
			pared = false;
		}
	}


	if (position.x <= 19 || position.x >= 276) {
		mov = 0;

		//aqui tendremos que llamar la accion del arbitro que envia el disco al player 1

		if (position.x <= 19) {
			if (position.y >= 94 && position.y <= 144) {
				App->player2->score += 5;
				arbitro = 1;
			}
			else {
				App->player2->score += 3;
				arbitro = 1;
			}
		}
		if (position.x >= 276) {
			if (position.y >= 94 && position.y <= 144) {
				App->player->score += 5;
				arbitro = 2;
			}
			else {
				App->player->score += 3;
				arbitro = 2;
			}
		}

		position.x = 150;
		position.y = 200;


	}


	currentAnimation2->Update();

	collider->SetPos(position.x, position.y);



	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleFrisbee::PostUpdate()
{
	SDL_Rect rect2 = currentAnimation2->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &rect2);
	return Update_Status::UPDATE_CONTINUE;
}

void ModuleFrisbee::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		arbitro = 0;

	}
}