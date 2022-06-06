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
#include "SceneBeachStage.h"
#include "SceneStageSelect.h"
#include <math.h>

ModuleFrisbee::ModuleFrisbee(bool startEnabled) : Module(startEnabled)
{
	//Moving animation
	for (int i = 0; i < 7; i++) {
		moving.PushBack({ 0 + (i * 51), 0, 51, 51 });
	}
	moving.loop = true;
	moving.speed = 0.2f;

	// Projectile motion animation
	for (int i = 0; i < 9; i++) {
		projectile.PushBack({ 51 + (i * 51), 102, 51, 51 });
	}
	for (int i = 0; i < 10; i++) {
		projectile.PushBack({ 0 + (i * 51), 153, 51, 51 });
	}
	for (int i = 0; i < 10; i++) {
		projectile.PushBack({ 0 + (i * 51), 204, 51, 51 });
	}
	for (int i = 0; i < 10; i++) {
		projectile.PushBack({ 0 + (i * 51), 255, 51, 51 });
	}
	for (int i = 0; i < 7; i++) {
		projectile.PushBack({ 0 + (i * 51), 306, 51, 51 });
	}
	projectile.loop = false;
	projectile.pingpong = false;
	projectile.speed = 0.7f;

	//Disco bloqueado
	blocked.PushBack({ 0, 102, 51, 51 });
	for (int i = 5; i >= 0; i--) {
		blocked.PushBack({ 204 + (i * 51), 51, 51, 51 });
	}
	for (int i = 0; i < 3; i++) {
		blocked.PushBack({ 357 + (i * 51), 0, 51, 51 });
	}
	for (int i = 0; i < 4; i++) {
		blocked.PushBack({ 0 + (i * 51), 51, 51, 51 });
	}
	blocked.loop = false;
	blocked.speed = 0.2f;  



	//Desaparece: que no hace falta hacer nada

	//Stop
	stop.PushBack({ 255, 0, 51, 51 });
	stop.loop = false;

}

ModuleFrisbee::~ModuleFrisbee()
{

}

bool ModuleFrisbee::Start()
{
	if (App->sceneBeachStage->loadFxOnce == 0) {
		catchFx = App->audio->LoadFx("Assets/Fx/Catch.wav");
		effectTossFx = App->audio->LoadFx("Assets/Fx/EffectToss.wav");
		frisbeeOnAirFX = App->audio->LoadFx("Assets/Fx/Freesbeonair.wav");
		landingFX = App->audio->LoadFx("Assets/Fx/Landing.wav");
		wallHitFX = App->audio->LoadFx("Assets/Fx/WallHit.wav");

		hiromiSuperFFX = App->audio->LoadFx("Assets/Fx/HiromiSuperF.wav");
		hiromiSuperSonicFX = App->audio->LoadFx("Assets/Fx/HiromiSupersonic1.wav");

		yooSuperSonicFX = App->audio->LoadFx("Assets/Fx/B.YooSuperSonic1.wav");
		yooSuperFFX = App->audio->LoadFx("Assets/Fx/B.YooSuperF.wav");

		wesselSuperSonicFX = App->audio->LoadFx("Assets/Fx/KlaussSupersonic1.wav");
		wesselSuperFFX = App->audio->LoadFx("Assets/Fx/KlaussSuperF.wav");
	}

	bool ret = true;
	speed = 1;
	arbitro = 1;
	FloorTime = 0;

	currentAnimation2 = &desaparece;
	LOG("Loading frisbee textures");

	if (App->sceneStageSelect->sceneSelected == Beach) {
		texture = App->textures->Load("Assets/Sprites/Levels/Frisbee.png");
	}
	else if (App->sceneStageSelect->sceneSelected == Lawn) {
		texture = App->textures->Load("Assets/Sprites/Levels/FrisbeeM.png");
	}
	else if (App->sceneStageSelect->sceneSelected == Concrete) {
		texture = App->textures->Load("Assets/Sprites/Levels/FrisbeeB.png");
	}



	position.x = App->sceneBeachStage->posicionXInicialFrisbee; 
	position.y = App->sceneBeachStage->posicionYInicialFrisbee;

	collider = App->collisions->AddCollider({ (int)position.x+16, (int)position.y+16, 16, 16 }, Collider::Type::FRISBEE, this);
	
	provisional = App->collisions->AddCollider({ 145, 52, 10, 16 }, Collider::Type::DEBUGG, this);
	provisional2 = App->collisions->AddCollider({ 145, 151, 10, 16 }, Collider::Type::DEBUGG, this);

	estadoF = ARBITROF;
	lanzamientoF = ARBITRO;
	estadoTF = INICIO;
	blockSuperShot = true;
	contadorBlock = 0;
	contador_Angulo_SuperShot = true;
	contadorYooSuperShot = 0;
	yooDirec = true;
	limiteConcrete = false;
	return ret;
}

Update_Status ModuleFrisbee::Update()
{

	switch (estadoF) {

	case ARBITROF:
		currentAnimation2 = &desaparece;
		break;

	case STOP:
		currentAnimation2 = &stop;
		break;

	case MOVIMIENTO:
		movimientoFrisbee();
		if (lanzamientoF != PARABOLA) {
			limitesFrisbee();
		}
		break;	

	case WITHPLAYER:
		currentAnimation2 = &desaparece;
		if (App->player->estadoP1 == ModulePlayer::estadoPlayer::WITHFRISBEE) {
			position.x = App->player->position.x + App->player->ajusteColliderX + App->player->ajusteColliderFrisbeeX;
			position.y = App->player->position.y;
		}
		else if (App->player2->estadoP2 == ModulePlayer2::estadoPlayer2::WITHFRISBEE) {
			position.x = App->player2->position.x - App->player2->ajusteColliderX - App->player2->ajusteColliderFrisbeeX;
			position.y = App->player2->position.y;;
		}
		break;

	case BLOCK:

		//animaciones del disco volando
		if (estadoTF == INICIO) {
			
			initialTimeF = SDL_GetTicks();

			if (lanzamientoF == BLOCKPLAYER1) {
				position.x = App->player->position.x + 22;
			}
			else if (lanzamientoF == BLOCKPLAYER2) {
				position.x = App->player2->position.x - 20;
			}

			timeLimitF = 1.2 * 1000;
			currentAnimation2 = &blocked;
			App->collisions->RemoveCollider(App->frisbee->collider);
			estadoTF = EJECUTANDO;

		}
		else if (estadoTF == EJECUTANDO) {
			timerF();
		}
		else if (estadoTF == FIN) {
			estadoTF = INICIO;
			collider = App->collisions->AddCollider({ (int)position.x+17, (int)position.y+17, 16,16 }, Collider::Type::FRISBEE, this);
			blockSuperShot = true;
			estadoF = SUELO;
			currentAnimation2->Reset();
		}
		break;

	case SUELO:
		currentAnimation2 = &stop;
		
		if (estadoTF == INICIO) {
			//lanzamientoF = NORMAL;
			initialTimeF = SDL_GetTicks();
			timeLimitF = 2 * 1000;
			App->audio->PlayFx(landingFX);
			estadoTF = EJECUTANDO;
		}
		else if (estadoTF == EJECUTANDO) {
			timerF();
		}
		else if (estadoTF == FIN) {
			estadoTF = INICIO;
			estadoF = ARBITROF;
			App->sceneBeachStage->Score();
			App->frisbee->limiteConcrete = false;
		}
		break;

	case(LANZAMIENTOARBITRO):
		if (estadoTF == INICIO)
		{
			App->sceneBeachStage->currentAnimationFrisbee->Reset();
			if (App->sceneBeachStage->arbitroFinalRonda == 1) {
				App->sceneBeachStage->currentAnimationFrisbee = &App->sceneBeachStage->lanzamientoIzquierda;
			}
			else if (App->sceneBeachStage->arbitroFinalRonda == 2) {
				App->sceneBeachStage->currentAnimationFrisbee = &App->sceneBeachStage->lanzamientoDerecha;
			}

			initialTimeF = SDL_GetTicks();
			timeLimitF = 1.1 * 1000;
			estadoTF = EJECUTANDO;
		}
		else if (estadoTF == EJECUTANDO) {
			timerF();
		}
		else if (estadoTF == FIN)
		{
			App->sceneBeachStage->Arbitro(App->sceneBeachStage->arbitroFinalRonda);
			estadoTF = INICIO;

		}
		break;

	}

//SUELO

	if (FloorTime == 120) {
		if (position.x < 150 && position.x >20) {
			App->player2->score += 2;
			if (App->sceneBeachStage->suddenDeath) {
				App->sceneBeachStage->Win();
			}
			/*App->sceneBeachStage->ScoreRound(1); - LO  CCOMENTO PARA Q NO DE ERRO*/
		}
		if (position.x > 150 && position.x <275) {
			App->player->score += 2;
			if (App->sceneBeachStage->suddenDeath) {
				App->sceneBeachStage->Win();
			}

			/*App->sceneBeachStage->ScoreRound(2);*/
		}
		position.x = App->sceneBeachStage->posicionXInicialFrisbee;
		position.y = App->sceneBeachStage->posicionYInicialFrisbee;
	}


	currentAnimation2->Update();
	collider->SetPos(position.x+17, position.y+17);

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleFrisbee::PostUpdate()
{
	SDL_Rect rect2 = currentAnimation2->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &rect2);

	if (App->input->keys[SDL_SCANCODE_F6] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(effectTossFx);
	}

	return Update_Status::UPDATE_CONTINUE;
}

void ModuleFrisbee::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && (c2 == App->player->collider || c2==App->player2->collider))
	{
		//lanzamientoF = NORMAL;
		//currentAnimation2 = &desaparece;
		//estadoF = estadoFrisbee::STOP;
	/*	FloorTime = 0;*/
		
		//Flash particle left or right depending on the player

		if (c2 == App->player->collider)
		{
			App->particles->AddParticle(0, 0, App->particles->leftGoalFlashParticle, App->player->position.x + 29, App->player->position.y, Collider::NONE, 0);
		}
		else if (c2 == App->player2->collider)
		{
			App->particles->AddParticle(0, 0, App->particles->rightGoalFlashParticle, App->player2->position.x - 5, App->player2->position.y, Collider::NONE, 0);
		}
		
		App->audio->PlayFx(catchFx);

		estadoTF = INICIO;

	}
}


void ModuleFrisbee :: movimientoFrisbee() {
	
	currentAnimation2 = &moving;
	
 	if (lanzamientoF == NORMAL) {
		if (direccionF == DARRIBA) {
			position.x += xspeed;
			position.y += yspeed;

		}
		else if (direccionF == DABAJO) {
			position.x += xspeed;
			position.y += yspeed;
		}
		else if (direccionF == HORIZONTAL) {
			position.x += xspeed;
		}
	}

	else if (lanzamientoF == PARABOLA) { //solo haremos que la parabola se pueda lanzar horizontalmente
		currentAnimation2 = &projectile;
		if (position.x < parabolaFinalX && indicacionPlayerParabola) {
			position.x += xspeed;
			if (position.y > parabolaFinalY && direccionF== DARRIBA) {
				position.y -= yspeed;
			}
			if (position.y < parabolaFinalY && direccionF == DABAJO) {
				position.y += yspeed;
			}
		}
		else if(position.x > parabolaFinalX && !indicacionPlayerParabola){
			position.x += xspeed;
			if (position.y > parabolaFinalY && direccionF == DARRIBA) {
				position.y -= yspeed;
			}
			if (position.y < parabolaFinalY && direccionF == DABAJO) {
				position.y += yspeed;
			}
		}
		else {
			collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 16, 16 }, Collider::Type::FRISBEE, this);
			estadoF = SUELO;
			currentAnimation2->Reset();
		}
	}

	else if (lanzamientoF == ARBITRO) {
		position.x += xspeed;
		position.y += yspeed;
	}

	else if (lanzamientoF == SUPERSHOT) {
		
		if (tipoSupershot == MITA_SUPERSHOT) {

			anguloSupershot();
			position.x += xspeed*0.8f;
			position.y += yspeed * cos(angulo);
			App->particles->AddParticle(0, 0, App->particles->mitaSuperShotParticle, position.x, position.y - 48, Collider::NONE, 5);
			App->audio->PlayFx(hiromiSuperSonicFX);
			App->audio->PlayFx(hiromiSuperFFX);


		} else if (tipoSupershot == YOO_SUPERSHOT) {
			if (contadorYooSuperShot <= 8 && contador_Angulo_SuperShot) {
				//if (App->player->estadoP1 == App->player->LANZAMIENTO_SUPER)
				//{
				//}
				//else if (App->player2->estadoP2 == App->player2->LANZAMIENTO_SUPER)
				//{
				//}
				if (App->frisbee->xspeed > 0)
				{
					App->particles->AddParticle(0, 0, App->particles->yooSuperShotParticleR, position.x, position.y + 20, Collider::NONE, 1);
				}
				else if (App->frisbee->xspeed < 0)
				{
					App->particles->AddParticle(0, 0, App->particles->yooSuperShotParticleL, position.x, position.y + 20, Collider::NONE, 1);
				}

				App->audio->PlayFx(yooSuperFFX);
				App->audio->PlayFx(yooSuperSonicFX);

				position.x += xspeed;
				contadorYooSuperShot++;

				if (contadorYooSuperShot == 8) {
					contador_Angulo_SuperShot = false;
					if (yooDirec) {
						contadorYooSuperShot = 0;
					}
					else if (!yooDirec) {
						contadorYooSuperShot = 4;
					}
					
					
				}
			}
			else if (!contador_Angulo_SuperShot) {

				if (yooDirec) {
					App->particles->AddParticle(0, 0, App->particles->yooSuperShotParticleU, position.x, position.y + 20, Collider::NONE, 0);
					position.y -= yspeed;
					contadorYooSuperShot++;

					if (contadorYooSuperShot >= 4 || position.y >= 170) {
						contador_Angulo_SuperShot = true;
						contadorYooSuperShot = 0;
						yooDirec = false;
					}

				}
				else if (!yooDirec) {
					App->particles->AddParticle(0, 0, App->particles->yooSuperShotParticleD, position.x, position.y + 20, Collider::NONE, 0);
					position.y += yspeed;
					contadorYooSuperShot--;

					if (contadorYooSuperShot <= 0 || position.y <= 45) {
						contador_Angulo_SuperShot = true;
						yooDirec = true;
					}
				}
			}

		}
		else if (tipoSupershot == WESSEL_SUPERSHOT) {

			if (App->frisbee->xspeed > 0 && App->frisbee->yspeed == 0)
			{
				App->particles->AddParticle(0, 0, App->particles->wesselSuperShotParticleR, position.x - 6, position.y + 8, Collider::NONE, 0);
			}
			else if (App->frisbee->xspeed < 0 && App->frisbee->yspeed == 0)
			{
				App->particles->AddParticle(0, 0, App->particles->wesselSuperShotParticleL, position.x - 6, position.y + 10, Collider::NONE, 0);
			}
			else if (App->frisbee->yspeed < 0)
			{
				App->particles->AddParticle(0, 0, App->particles->wesselSuperShotParticleU, position.x + 16, position.y - 5, Collider::NONE, 0);
			}
			else if (App->frisbee->yspeed > 0)
			{
				App->particles->AddParticle(0, 0, App->particles->wesselSuperShotParticleD, position.x, position.y, Collider::NONE, 0);
			}

			App->audio->PlayFx(wesselSuperSonicFX);
			App->audio->PlayFx(wesselSuperFFX);

			if (position.x < limiteWesselSupershot && indicacionPlayerParabola) {
				position.x += xspeed;
			} 
			else if (position.x >= limiteWesselSupershot && indicacionPlayerParabola){
				position.y += yspeed;
			}

			if (position.x > limiteWesselSupershot && !indicacionPlayerParabola) {
				position.x += xspeed;
			}
			else if (position.x <= limiteWesselSupershot && !indicacionPlayerParabola) {
				position.y += yspeed;
			}
		}
	}



}

void ModuleFrisbee::limitesFrisbee() {

	if (position.x < 19 || position.x >276) {

		//funsion score
		estadoF = ARBITROF;
		App->player->estadoP1 = ModulePlayer::estadoPlayer::STOP;
		App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::STOP;
		App->sceneBeachStage->Score();
		tipoSupershot = NONE;

	}

	if (tipoSupershot == WESSEL_SUPERSHOT) {
		
		if (position.y <= App->sceneBeachStage->limiteSuperior) {
			position.x += xspeed;
			yspeed = 0;
		}
		else if (position.y >= App->sceneBeachStage->limiteInferior) {
			position.x += xspeed;
			yspeed = 0;
		}

	}
	else if (lanzamientoF != ARBITRO && lanzamientoF != SUPERSHOT) {
		if (position.x >= 19 && position.x <= 276) {
			
			if (App->sceneStageSelect->sceneSelected == Concrete) {

				//UP
				if (position.y <= App->sceneBeachStage->limiteSuperior-4) { //48
					App->audio->PlayFx(wallHitFX);
					//Right
					if (xspeed > 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocDownright, position.x + 15, position.y+5, Collider::NONE, 0);
					}
					//Left	
					else if (xspeed < 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocDownleft, position.x - 25, position.y+5, Collider::NONE, 0);
					}
					yspeed *= -1;
				}
				//DOWN
				else if (position.y >= App->sceneBeachStage->limiteInferior+11) { //170
					App->audio->PlayFx(wallHitFX);
					//Right
					if (xspeed > 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocUpright, position.x + 15, position.y+28, Collider::NONE, 0);
					}
					//Left	
					else if (xspeed < 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocUpleft, position.x - 25, position.y+28, Collider::NONE, 0);
					}
					yspeed *= -1;
				}
			
			}
			else if (App->sceneStageSelect->sceneSelected == Beach) {

				//UP
				if (position.y <= App->sceneBeachStage->limiteSuperior-15) { //48
					App->audio->PlayFx(wallHitFX);
					//Right
					if (xspeed > 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocDownright, position.x, position.y+18, Collider::NONE, 0); //15
					}
					//Left	
					else if (xspeed < 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocDownleft, position.x, position.y+18, Collider::NONE, 0); //-25
					}
					yspeed *= -1;
				}
				//DOWN
				else if (position.y >= App->sceneBeachStage->limiteInferior+15) { //170
					App->audio->PlayFx(wallHitFX);
					//Right
					if (xspeed > 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocUpright, position.x, position.y+14, Collider::NONE, 0); //15
					}
					//Left	
					else if (xspeed < 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocUpleft, position.x, position.y+14, Collider::NONE, 0); //-25
					}
					yspeed *= -1;
				}

			}
			else if (App->sceneStageSelect->sceneSelected == Lawn) {

				//UP
				if (position.y <= App->sceneBeachStage->limiteSuperior - 15) { //48
					App->audio->PlayFx(wallHitFX);
					//Right
					if (xspeed > 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocDownright, position.x+5, position.y + 29, Collider::NONE, 0); //15
					}
					//Left	
					else if (xspeed < 0)
					{
						App->particles->AddParticle(0, 0, App->particles->xocDownleft, position.x, position.y + 29, Collider::NONE, 0); //-25
					}
					yspeed *= -1;
				}
				//DOWN
				else if (position.y >= App->sceneBeachStage->limiteInferior + 15) { //170
					App->audio->PlayFx(wallHitFX);
					//Right
					if (xspeed > 0 )
					{
						App->particles->AddParticle(0, 0, App->particles->xocUpright, position.x, position.y + 10, Collider::NONE, 0); //15
					}
					//Left	
					else if (xspeed < 0 )
					{
						App->particles->AddParticle(0, 0, App->particles->xocUpleft, position.x, position.y + 10, Collider::NONE, 0); //-25
					}
					yspeed *= -1;
				}

			}
			
			
			
		}
	}

	if (App->sceneStageSelect->sceneSelected == Concrete) {

		if (position.x <= 155-20 && position.x >= 145-20) {
			if (((position.y <= 62 && position.y >= 52) || (position.y <= 161 && position.y >= 151)) && limiteConcrete) {
				limiteConcrete = false;
				yspeed *= -1;
			}
		}
	}
}


void ModuleFrisbee::timerF() {
	currentTimeF = SDL_GetTicks();

	if (currentTimeF - initialTimeF >= timeLimitF) {
		estadoTF = estadoTimerF::FIN;
	}
}

void ModuleFrisbee::vel_parabola(int pos_Player, int pos_final_frisbee) {
	if (pos_final_frisbee >= App->sceneBeachStage->limiteCentralDer+5 && pos_final_frisbee <=230){
		projectile.speed = 54 / ((pos_final_frisbee - pos_Player) / xspeed);
	}
	else if (pos_final_frisbee >= 23 && pos_final_frisbee <= App->sceneBeachStage->limiteCentralIzq - 5) {
		projectile.speed = 66 / ((-pos_final_frisbee - pos_Player) / xspeed);
	}

}

void ModuleFrisbee::anguloSupershot() {

	if (angulo <= 180 && contador_Angulo_SuperShot) { 
		angulo -= 0.15;
		if (angulo == 0) {
			contador_Angulo_SuperShot = false;
		}
	}
	else if (angulo >= 0 && !contador_Angulo_SuperShot) {
		angulo += 0.15;
		if (angulo == 180) {
			contador_Angulo_SuperShot = true;
		}
	}



}

bool ModuleFrisbee::CleanUp() {
	//1
	App->textures->Unload(texture);
	App->collisions->RemoveCollider(collider);
	App->collisions->RemoveCollider(provisional);
	App->collisions->RemoveCollider(provisional2);
	//App->audio->CleanFX("Assets/Fx/Catch.wav");

	return true;
}
