#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = App->textures->Load("Assets/Sprites/Levels/Beach.png");

	//Frisbee Player 1 Basic Mov
	frisbee.anim.PushBack({ 117, 48, 16, 16 });
	frisbee.anim.PushBack({ 149, 48, 16, 16 });
	frisbee.anim.PushBack({ 181, 48, 16, 16 });
	frisbee.anim.PushBack({ 213, 48, 16, 16 });
	frisbee.speed.x = 5;
	frisbee.lifetime = 180;
	frisbee.anim.speed = 0.2f;

	frisbeeUp.anim.PushBack({ 117, 48, 16, 16 });
	frisbeeUp.anim.PushBack({ 149, 48, 16, 16 });
	frisbeeUp.anim.PushBack({ 181, 48, 16, 16 });
	frisbeeUp.anim.PushBack({ 213, 48, 16, 16 });
	frisbeeUp.speed.x = 5;
	frisbeeUp.speed.y = -5;
	frisbeeUp.lifetime = 180;
	frisbeeUp.anim.speed = 0.2f;

	frisbeeDown.anim.PushBack({ 117, 48, 16, 16 });
	frisbeeDown.anim.PushBack({ 149, 48, 16, 16 });
	frisbeeDown.anim.PushBack({ 181, 48, 16, 16 });
	frisbeeDown.anim.PushBack({ 213, 48, 16, 16 });
	frisbeeDown.speed.x = 5;
	frisbeeDown.speed.y = 5;
	frisbeeDown.lifetime = 180;
	frisbeeDown.anim.speed = 0.2f;

	//Frisbee Player 2 Basic Mov
	frisbee2.anim.PushBack({ 117, 48, 16, 16 });
	frisbee2.anim.PushBack({ 149, 48, 16, 16 });
	frisbee2.anim.PushBack({ 181, 48, 16, 16 });
	frisbee2.anim.PushBack({ 213, 48, 16, 16 });
	frisbee2.speed.x = -5;
	frisbee.lifetime = 180;
	frisbee.anim.speed = 0.2f;

	frisbee2Up.anim.PushBack({ 117, 48, 16, 16 });
	frisbee2Up.anim.PushBack({ 149, 48, 16, 16 });
	frisbee2Up.anim.PushBack({ 181, 48, 16, 16 });
	frisbee2Up.anim.PushBack({ 213, 48, 16, 16 });
	frisbee2Up.speed.x = -5;
	frisbee2Up.speed.y = -5;
	frisbee2Up.lifetime = 180;
	frisbee2Up.anim.speed = 0.2f;

	frisbee2Down.anim.PushBack({ 117, 48, 16, 16 });
	frisbee2Down.anim.PushBack({ 149, 48, 16, 16 });
	frisbee2Down.anim.PushBack({ 181, 48, 16, 16 });
	frisbee2Down.anim.PushBack({ 213, 48, 16, 16 });
	frisbee2Down.speed.x = -5;
	frisbee2Down.speed.y = 5;
	frisbee2Down.lifetime = 180;
	frisbee2Down.anim.speed = 0.2f;

	//Frisbee Player 1 Projectile motion animation
	frisbeeProjectile.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectile.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectile.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectile.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectile.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectile.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectile.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectile.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectile.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectile.speed.x = 2,5;
	frisbeeProjectile.anim.loop = false;
	frisbeeProjectile.lifetime = 180;
	frisbeeProjectile.anim.speed = 0.1f;

	frisbeeProjectile.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectile.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectile.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectile.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectile.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectile.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectile.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectile.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectile.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectile.speed.x = 2, 5;
	frisbeeProjectile.anim.loop = false;
	frisbeeProjectile.lifetime = 180;
	frisbeeProjectile.anim.speed = 0.1f;

	frisbeeProjectileUp.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectileUp.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectileUp.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectileUp.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectileUp.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectileUp.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectileUp.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectileUp.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectileUp.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectileUp.speed.x = 2, 5;
	frisbeeProjectileUp.speed.y = -1;
	frisbeeProjectileUp.anim.loop = false;
	frisbeeProjectileUp.lifetime = 180;
	frisbeeProjectileUp.anim.speed = 0.1f;

	frisbeeProjectileDown.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectileDown.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectileDown.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectileDown.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectileDown.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectileDown.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectileDown.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectileDown.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectileDown.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectileDown.speed.x = 2, 5;
	frisbeeProjectileDown.speed.y = 1;
	frisbeeProjectileDown.anim.loop = false;
	frisbeeProjectileDown.lifetime = 180;
	frisbeeProjectileDown.anim.speed = 0.1f;

	//Frisbee Player 2 Projectile motion animation
	frisbeeProjectile2.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectile2.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectile2.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectile2.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectile2.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectile2.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectile2.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectile2.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectile2.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectile2.speed.x = -2.5; 
	frisbeeProjectile2.anim.loop = false;
	frisbeeProjectile2.lifetime = 180;
	frisbeeProjectile2.anim.speed = 0.1f;

	frisbeeProjectile2Up.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectile2Up.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectile2Up.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectile2Up.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectile2Up.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectile2Up.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectile2Up.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectile2Up.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectile2Up.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectile2Up.speed.x = -2, 5;
	frisbeeProjectile2Up.speed.y = -1;
	frisbeeProjectile2Up.anim.loop = false;
	frisbeeProjectile2Up.lifetime = 180;
	frisbeeProjectile2Up.anim.speed = 0.1f;

	frisbeeProjectile2Down.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectile2Down.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectile2Down.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectile2Down.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectile2Down.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectile2Down.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectile2Down.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectile2Down.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectile2Down.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectile2Down.speed.x = -2, 5;
	frisbeeProjectile2Down.speed.y = 1;
	frisbeeProjectile2Down.anim.loop = false;
	frisbeeProjectile2Down.lifetime = 180;
	frisbeeProjectile2Down.anim.speed = 0.1f;

	return true;
}

Update_Status ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			particles[i]->pendingToDelete = true;
			particles[i]->collider->pendingToDelete = true;
			break;
		}
	}
}

Update_Status ModuleParticles::Update()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if(particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if(particle->Update() == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			newParticle->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE)
				newParticle->collider = App->collisions->AddCollider(newParticle->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}