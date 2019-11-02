#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleScenePinball : public Module
{
public:
	ModuleScenePinball(Application* app, bool start_enabled = true);
	~ModuleScenePinball();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Map_shape();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* flipper_right1;
	PhysBody* point_right1;	
	PhysBody* flipper_right2;
	PhysBody* point_right2;	
	PhysBody* flipper_right3;
	PhysBody* point_right3;
	PhysBody* flipper_left1;
	PhysBody* point_left1;
	PhysBody* flipper_left2;
	PhysBody* point_left2;	
	PhysBody* flipper_left3;
	PhysBody* point_left3;

	PhysBody* launcher;

	p2List<PhysBody*> balls;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* ball;
	SDL_Texture* background;


	uint bonus_fx;
	uint Soundtrack[2];
	uint launcher_fx;
	uint flipper_fx;
	p2Point<int> ray;
	bool ray_on;
};
