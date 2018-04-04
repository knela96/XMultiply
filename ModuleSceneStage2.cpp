#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleSceneStage2.h"
#include "ModulePlayer.h"

ModuleSceneStage2::ModuleSceneStage2()
{
	//ground
	ground.x = 0;
	ground.y = 0;
	ground.w = 512;
	ground.h = 512;

	// Background / sky
	background.x = 0;
	background.y = 1;
	background.w = 4961;
	background.h = 513;
}

ModuleSceneStage2::~ModuleSceneStage2()
{}

// Load assets
bool ModuleSceneStage2::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	graphics = App->textures->Load("Assets/TileMap.png");
	back = App->textures->Load("Assets/FirstLvlMap3.png");
	App->player->Enable();

	return ret;
}

// Update: draw background
update_status ModuleSceneStage2::Update()
{
	--ScrollingOffset;
	if (ScrollingOffset < -512)
	{
		ScrollingOffset = 0;
	}


	App->render->Blit(back, ScrollingOffset, 0, &ground, 0.1f);
	App->render->Blit(back, ScrollingOffset + 512, 0, &ground, 0.1f);


	App->render->Blit(graphics, 0, 0, &background);




	return UPDATE_CONTINUE;
}