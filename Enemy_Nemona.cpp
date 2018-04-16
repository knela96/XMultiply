#include "Application.h"
#include "Enemy_Nemona.h"
#include "ModuleCollision.h"

Enemy_Nemona::Enemy_Nemona(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 0,0,48,48 });
	fly.PushBack({ 49,0,50,48 });
	fly.PushBack({ 113,0,50,48 });
	fly.speed = 0.01f;

	animation = &fly;

	path->PushBack({ 0 , 0 }, 2, &fly);

	collider = App->collision->AddCollider({ 0, 0, 48, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	original_position.x = x;
	original_position.y = y;
}

void Enemy_Nemona::Move()
{
	position = original_position + path->GetCurrentPosition(&animation);
}
