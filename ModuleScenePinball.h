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

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	p2List<PhysBody*> balls;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* ball;
	SDL_Texture* background;


	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
