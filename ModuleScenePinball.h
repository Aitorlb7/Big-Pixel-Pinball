#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"

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
	p2List<PhysBody*> chains;


	PhysBody* right;
	PhysBody* point_right;
	PhysBody* left;
	PhysBody* point_left;
	PhysBody* left_flipper_joint;
	PhysBody* right_flipper_joint;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;




	PhysBody* launcher;

	p2List<PhysBody*> balls;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* ball;
	SDL_Texture* background;
	SDL_Texture* flipper_right_tex;
	SDL_Texture* flipper_left_tex;

	PhysBody* flipper_right_body = new PhysBody();


	uint bonus_fx;
	uint Soundtrack[2];
	uint launcher_fx;
	uint flipper_fx;
	p2Point<int> ray;
	bool ray_on;
};
