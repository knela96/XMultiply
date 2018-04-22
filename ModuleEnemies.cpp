#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "Enemy.h"
#include "Enemy_BrownWorm.h"
#include "Enemy_LittleShrimp.h"
#include "Enemy_PowerupShip.h"
#include "Enemy_Nemona.h"
#include "ModuleAudio.h"

#define SPAWN_MARGIN 50

ModuleEnemies::ModuleEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
	for (uint i = 0; i < MAX_TEXTURES; ++i)
		sprites[i] = nullptr;
}

// Destructor
ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites[ENEMY_TYPES::BROWN_WORM] = App->textures->Load("Assets/Sprites/Stage1/Enemies/monsterball.png");
	sprites[ENEMY_TYPES::LITTLE_SHRIMP] = App->textures->Load("Assets/Sprites/Stage1/Enemies/littleshrimp.png");
	sprites[ENEMY_TYPES::NEMONA_TENTACLE] = App->textures->Load("Assets/Sprites/Stage1/Enemies/nemona.png");
	sprites[ENEMY_TYPES::POWERUPSHIP] = App->textures->Load("Assets/Sprites/PowerUp/PowerUp.png");

	Brownworm_fx = App->audio->LoadS("Assets/Audio Files/SFX in WAV/xmultipl-100.wav");
	LittleShrimp_fx = App->audio->LoadS("Assets/Audio Files/SFX in WAV/xmultipl-100.wav");
	Nemona_fx = App->audio->LoadS("Assets/Audio Files/SFX in WAV/xmultipl-094.wav");
	Powership_fx = App->audio->LoadS("Assets/Audio Files/SFX in WAV/xmultipl-055.wav");

	return true;
}

update_status ModuleEnemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			
			if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleEnemies::Update()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) {
			switch(enemies[i]->type) {
			case BROWN_WORM:
				enemies[i]->Draw(sprites[BROWN_WORM]);
				break;
			case LITTLE_SHRIMP:
				enemies[i]->Draw(sprites[LITTLE_SHRIMP]);
				break;
			case NEMONA_TENTACLE:
				enemies[i]->Draw(sprites[NEMONA_TENTACLE]);
				break;
			case POWERUPSHIP:
				enemies[i]->Draw(sprites[POWERUPSHIP]);
				break;
			}
		}

	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");
	App->audio->UnloadS(Brownworm_fx);
	App->audio->UnloadS(LittleShrimp_fx);
	App->audio->UnloadS(Nemona_fx);
	App->audio->UnloadS(Powership_fx);

	for (uint i = 0; i < MAX_TEXTURES; ++i) {
		if (sprites[i] != nullptr)
		{
			App->textures->Unload(sprites[i]);
		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y, uint PU, bool move_up)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].move_up = move_up;
			queue[i].x = x;
			queue[i].y = y;
			queue[i].PU = PU;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::SpawnEnemy(const EnemyInfo& info)
{
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::BROWN_WORM:
			enemies[i] = new Enemy_BrownWorm(info.x, info.y, info.move_up);
			enemies[i]->type = ENEMY_TYPES::BROWN_WORM;
			break;
		case ENEMY_TYPES::LITTLE_SHRIMP:
			enemies[i] = new Enemy_LittleShrimp(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::LITTLE_SHRIMP;
			break;
		case ENEMY_TYPES::NEMONA_TENTACLE:
			enemies[i] = new Enemy_Nemona(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::NEMONA_TENTACLE;
			break;
		case ENEMY_TYPES::POWERUPSHIP:
			enemies[i] = new Enemy_PowerupShip(info.x, info.y, info.PU);
			enemies[i]->type = ENEMY_TYPES::POWERUPSHIP;
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1 && (c2->type == COLLIDER_PLAYER || c2->type == COLLIDER_PLAYER_SHOT))
		{
			switch (enemies[i]->type) {
				case ENEMY_TYPES::BROWN_WORM:
					App->audio->PlaySound(Brownworm_fx);
					break;
				case ENEMY_TYPES::LITTLE_SHRIMP:
					App->audio->PlaySound(LittleShrimp_fx);
					break;
				case ENEMY_TYPES::NEMONA_TENTACLE:
					App->audio->PlaySound(Nemona_fx);
					break;
				case ENEMY_TYPES::POWERUPSHIP:
					App->audio->PlaySound(Powership_fx);

					break;
			}
			enemies[i]->OnCollision(c2);
			delete enemies[i];
			enemies[i] = nullptr;
			break;
		}
	}
}
