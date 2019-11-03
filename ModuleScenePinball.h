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
	void Spring();
	void Ball_respawn();
	void Sensed();
	void Create100(int x, int y);

public:
	int sensed_timer;
	bool start_timer;

	int x = 0;
	int y = 0;
	int spring_y = 1023;
	int ball_x = 0;
	int ball_y = 0;
	int numBalls = 0;
	
	

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> chains;

	PhysBody* sensor_blue;
	PhysBody* sensor_orange;
	PhysBody* sensor_green;	
	PhysBody* sensor_red;
	PhysBody* sensor_pink;
	PhysBody* sensor_yellow;

	bool blue_sensed;
	bool green_sensed;
	bool orange_sensed;
	bool pink_sensed;
	bool red_sensed;
	bool yellow_sensed;

	bool blit_blue;
	bool blit_green;
	bool blit_orange;
	bool blit_pink;
	bool blit_red;
	bool blit_yellow;



	PhysBody* ball;
	PhysBody* spring;
	PhysBody* left_flipper_joint;
	PhysBody* right_flipper_joint;
	PhysBody* staticSpring;
	PhysBody* flipper_right_body = new PhysBody();
	PhysBody* flipper_left_body = new PhysBody();
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;
	b2PrismaticJointDef prismaticJoint_spring;



	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* ball_tex;
	SDL_Texture* background;
	SDL_Texture* flipper_right_tex;
	SDL_Texture* flipper_left_tex;
	SDL_Texture* spring_tex;
	SDL_Texture* sensor_blue_tex;
	SDL_Texture* sensor_orange_tex;
	SDL_Texture* sensor_green_tex;	
	SDL_Texture* sensor_pink_tex;
	SDL_Texture* sensor_red_tex;
	SDL_Texture* sensor_yellow_tex;
	SDL_Texture* sensor_green_tex;
	SDL_Texture* plus100;


	uint bonus_fx;
	uint Soundtrack;
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
	int Background[90] = {
	617, 1207,
	614, 856,
	605, 819,
	577, 777,
	536, 740,
	536, 727,
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
	257, 85,
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
	208, 1175,
	200, 1435,
	396, 1436,
	408, 1175,
	447, 1147,
	560, 1083,
	560, 1287,
	628, 1288
	};
	int ShapeR1[12] = {
	411, 1104,
	407, 1074,
	505, 1020,
	520, 990,
	525, 1025,
	503, 1057
	};
	int ShapeL1[12] = {
	196, 1074,
	193, 1105,
	98, 1055,
	82, 1024,
	85, 986,
	101, 1018
	};


	int ShapeR2[12] = {
		408, 980,
		464, 879,
		480, 883,
		476, 958,
		419, 988,
		408, 981
	};

	int ShapeL2[10] = {
	133, 957,
	128, 884,
	144, 880,
	202, 976,
	192, 986
	};
	int Start_Tube[22] = {
	565, 1204,
	564, 876,
	548, 824,
	523, 795,
	510, 785,
	515, 778,
	529, 784,
	557, 814,
	572, 840,
	578, 874,
	580, 1207
	};
	int SpinTube[48] = {
	119, 833,
	118, 706,
	93, 668,
	86, 171,
	89, 110,
	110, 79,
	153, 60,
	178, 66,
	190, 77,
	199, 122,
	184, 161,
	179, 172,
	156, 140,
	138, 116,
	107, 127,
	130, 204,
	96, 299,
	103, 472,
	130, 526,
	113, 541,
	135, 594,
	102, 661,
	125, 700,
	130, 832
	};
	int Circle1[40] = {
	367, 529,
	380, 529,
	390, 533,
	400, 540,
	407, 548,
	410, 556,
	412, 568,
	412, 574,
	410, 583,
	404, 593,
	394, 603,
	381, 608,
	365, 608,
	351, 604,
	340, 595,
	333, 582,
	332, 564,
	337, 548,
	345, 538,
	356, 532
	};
	int Circle2[40] = {
    515, 392,
    527, 387,
    545, 387,
    556, 391,
    565, 398,
    570, 403,
    574, 414,
    576, 425,
    574, 440,
    568, 451,
    558, 461,
    546, 466,
    531, 467,
    517, 463,
    506, 454,
    499, 444,
    496, 433,
    497, 418,
    502, 405,
    508, 397
	};
};
