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
	void Flippers();

public:
	int x = 0;
	int y = 0;
	
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> chains;


	PhysBody* right;
	PhysBody* point_right;
	PhysBody* left;
	PhysBody* point_left;
	PhysBody* left_flipper_joint;
	PhysBody* right_flipper_joint;
	PhysBody* flipper_right_body = new PhysBody();
	PhysBody* flipper_left_body = new PhysBody();
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



	uint bonus_fx;
	uint Soundtrack[2];
	uint launcher_fx;
	uint flipper_fx;
	p2Point<int> ray;
	bool ray_on;




	
	int Flipper_Left[18] = {
	0, 10,
	0, 25,
	7, 34,
	60, 63,
	70, 58,
	70, 47,
	23, 0,
	8, 0,
	0, 10
	};
	int Flipper_Right[20] = {
	47, 2,
	0, 46,
	0, 55,
	4, 62,
	11, 62,
	67, 30,
	70, 23,
	68, 10,
	60, 2,
	47, 2
	};
	int Background[82] = {
	630, 1207,
	622, 815,
	595, 764,
	571, 744,
	549, 731,
	613, 633,
	617, 263,
	608, 225,
	565, 250,
	552, 237,
	597, 210,
	613, 181,
	610, 80,
	548, 30,
	417, 30,
	363, 77,
	355, 177,
	363, 205,
	407, 233,
	398, 254,
	352, 227,
	311, 153,
	266, 124,
	227, 47,
	180, 27,
	113, 29,
	78, 53,
	50, 92,
	45, 132,
	45, 655,
	56, 691,
	67, 718,
	73, 826,
	37, 836,
	43, 1080,
	199, 1172,
	199, 1435,
	393, 1443,
	405, 1173,
	562, 1077,
	560, 1233
	};
};
