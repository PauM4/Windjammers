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

	// Explosion particle
	explosion.anim.PushBack({274, 296, 33, 30});
	explosion.anim.PushBack({313, 296, 33, 30});
	explosion.anim.PushBack({346, 296, 33, 30});
	explosion.anim.PushBack({382, 296, 33, 30});
	explosion.anim.PushBack({419, 296, 33, 30});
	explosion.anim.PushBack({457, 296, 33, 30});
	explosion.anim.loop = false;
	explosion.anim.speed = 0.3f;

	laser.anim.PushBack({ 232, 103, 16, 12 });
	laser.anim.PushBack({ 249, 103, 16, 12 });
	laser.speed.x = 5;
	laser.lifetime = 180;
	laser.anim.speed = 0.2f;

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