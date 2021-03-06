#ifndef __ModuleEnemy_H__
#define __ModuleEnemy_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
typedef unsigned int Uint32;

struct Enemy {
	SDL_Texture* graphics = nullptr;
	Animation forward;
	iPoint position;
	Collider* collider;
	bool dead = false;
};

class ModuleEnemy : public Module
{
public:

	ModuleEnemy();
	~ModuleEnemy();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* collider1, Collider* collider2);

public:
	Enemy enemies[30];
	Enemy Powerups[15];
	Uint32* start_time = 0;
	Uint32* spawn_delay;


	Animation anim1;
	Animation animpower;
};

#endif // __ModuleEnemy_H__