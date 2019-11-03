#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleScenePinball.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include"ModuleUI.h"

ModuleScenePinball::ModuleScenePinball(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
}

ModuleScenePinball::~ModuleScenePinball()
{}

// Load assets
bool ModuleScenePinball::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x =  0;
	App->renderer->camera.y = -673; //Magic number


	ball = App->physics->CreateCircle(592, 845, 15, b2_dynamicBody);


	Map_shape();
	Flippers();
	Spring();
	


	background = App->textures->Load("Textures/background.png");
	ball_tex = App->textures->Load("Textures/ball.png");
	flipper_right_tex = App->textures->Load("Textures/Flipper_Right.png");
	flipper_left_tex = App->textures->Load("Textures/Flipper_Left.png");
	spring_tex = App->textures->Load("Textures/Spring.png");
	sensor_blue_tex = App->textures->Load("Textures/Blue_Active.png");
	sensor_orange_tex = App->textures->Load("Textures/Orange_Active.png");
	sensor_green_tex = App->textures->Load("Textures/Green_Active.png");
	sensor_red_tex = App->textures->Load("Textures/Red_Active.png");
	sensor_pink_tex = App->textures->Load("Textures/Pink_Active.png");
	sensor_yellow_tex = App->textures->Load("Textures/Yellow_Active.png");



	App->audio->PlayMusic("Audio/MainTheme.wav", -1.0f);
	launcher_fx = App->audio->LoadFx("Audio/Launcher_fx.wav");
	flipper_fx = App->audio->LoadFx("Audio/Flipper_fx.wav");


	sensor_blue = App->physics->CreateRectangleSensor(237, 913, 60, 60);
	sensor_orange = App->physics->CreateRectangleSensor(310, 1035, 60, 60);
	sensor_green = App->physics->CreateRectangleSensor(380, 913, 60, 60);
	sensor_pink = App->physics->CreateRectangleSensor(320, 690, 60, 60);
	sensor_red = App->physics->CreateRectangleSensor(450, 720, 60, 60);
	sensor_yellow = App->physics->CreateRectangleSensor(170, 720, 60, 60);


	

	return ret;
}

// Load assets
bool ModuleScenePinball::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleScenePinball::Update()
{
	if (start_timer)
		sensed_timer++;
	Sensed();
	App->renderer->Blit(background, 0, 0);
	
	static int force = 0;
	static int elastic_force = 1;
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())), 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		force += 20;
		if (force > 380)
			force = 380;
		if(spring_y<1029)
			++spring_y;
		//prismaticJoint_spring.lowerTranslation -= PIXEL_TO_METERS(elastic_force);
		//prismaticJoint_spring.upperTranslation -= PIXEL_TO_METERS(elastic_force);

	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		spring->body->ApplyForceToCenter(b2Vec2(0, -force), 1);
		spring_y = 1023;
		App->audio->PlayFx(launcher_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipper_right_body->body->ApplyTorque(100, true);
		App->audio->PlayFx(flipper_fx);
	}
	else
	{
		flipper_right_body->body->ApplyTorque(-30, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		flipper_left_body->body->ApplyTorque(-100, true);
		App->audio->PlayFx(flipper_fx);
	}
	else
	{
		flipper_left_body->body->ApplyTorque(30, true);
	}

	// Prepare for raycast ------------------------------------------------------
	

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* b = boxes.getFirst();
	

	while(b != NULL)
	{
		int x, y;
		b->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, b->data->GetRotation());
		b = b->next;
	}

	b = chains.getFirst();

	while(b != NULL)
	{
		int x, y;
		b->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, b->data->GetRotation());
		b = b->next;
	}
	
	//CAMERA--------------------------
	
	ball->GetPosition(ball_x, ball_y);

	App->renderer->camera.y = -ball_y + (App->renderer->camera.h / 2);
	if (App->renderer->camera.y < -673)
	{
		App->renderer->camera.y = -673;
	}
	else if (App->renderer->camera.y > 0)
	{
		App->renderer->camera.y = 0;
	}
	//------------------------------------
	Ball_respawn();
	App->renderer->Blit(spring_tex, 585, spring_y);
	
	flipper_left_body->GetPosition(x, y);
	App->renderer->Blit(flipper_left_tex, x, y, NULL, 1.0f, flipper_left_body->GetRotation(), 0, 0);

	flipper_right_body->GetPosition(x, y);
	App->renderer->Blit(flipper_right_tex, x, y, NULL, 1.0f, flipper_right_body->GetRotation(), 0, 0);


	if(blit_blue)
		App->renderer->Blit(sensor_blue_tex, 204, 880);

	if (blit_green)
		App->renderer->Blit(sensor_green_tex, 338, 880);

	if (blit_orange)
		App->renderer->Blit(sensor_orange_tex, 271, 1004);
	
	if (blit_pink)
		App->renderer->Blit(sensor_pink_tex, 281, 664);	
	
	if (blit_red)
		App->renderer->Blit(sensor_red_tex, 425, 699);	
	
	if (blit_yellow)
		App->renderer->Blit(sensor_yellow_tex, 142, 699);

	y100 -= sensed_timer / 40;
	if (sensed_timer < 120)
		App->renderer->Blit(plus100, x100, y100);
	if (sensed_timer > 120)
	{
		start_timer = false;
		sensed_timer = 0;
		App->textures->Unload(plus100);
		plus100 = nullptr;
	}

	App->renderer->Blit(ball_tex, ball_x, ball_y, NULL, 1.0f);

	return UPDATE_CONTINUE;
}

void ModuleScenePinball::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}



void ModuleScenePinball::Flippers()
{
	//Right Flipper---------------------------------------------

	flipper_right_body = App->physics->CreateFlippers(393,1098,Flipper_Right,17, b2_dynamicBody);
	//--------------------------------
	right_flipper_joint = App->physics->CreateCircle(393, 1098, 2, b2_staticBody);

	b2RevoluteJointDef revoluteJointDef_right;
	revoluteJointDef_right.bodyA = right_flipper_joint->body;
	revoluteJointDef_right.bodyB = flipper_right_body->body;
	revoluteJointDef_right.collideConnected = false;

	revoluteJointDef_right.enableLimit = true;
	revoluteJointDef_right.lowerAngle = DEGTORAD;
	revoluteJointDef_right.upperAngle = 60 * DEGTORAD;

	revoluteJointDef_right.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef_right.localAnchorB.Set(PIXEL_TO_METERS(55), PIXEL_TO_METERS(15));

	b2RevoluteJoint* right_flipper_joint;
	right_flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_right);



	//Left Flipper---------------------------------------------
	flipper_left_body = App->physics->CreateFlippers(206, 1094, Flipper_Left, 17, b2_dynamicBody);
	
	//--------------------------------
	left_flipper_joint = App->physics->CreateCircle(206, 1094, 2, b2_staticBody);

	b2RevoluteJointDef revoluteJointDef_left;
	revoluteJointDef_left.bodyA = left_flipper_joint->body;
	revoluteJointDef_left.bodyB = flipper_left_body->body;
	revoluteJointDef_left.collideConnected = false;

	revoluteJointDef_left.enableLimit = true;
	revoluteJointDef_left.lowerAngle = -60 * DEGTORAD;
	revoluteJointDef_left.upperAngle =  DEGTORAD;

	revoluteJointDef_left.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef_left.localAnchorB.Set(PIXEL_TO_METERS(7), PIXEL_TO_METERS(12));

	b2RevoluteJoint* left_flipper_joint;
	left_flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_left);

}
void ModuleScenePinball::Spring()
{

	spring = App->physics->CreateRectangle(592, 850, 30, 15);
	staticSpring = App->physics->CreateRectangle(592, 1150, 30, 15);
	staticSpring->body->SetType(b2_staticBody);

	prismaticJoint_spring.bodyB = spring->body;
	prismaticJoint_spring.bodyA = staticSpring->body;


	prismaticJoint_spring.collideConnected = false;
	prismaticJoint_spring.enableLimit = true;

	prismaticJoint_spring.lowerTranslation = PIXEL_TO_METERS(112);
	prismaticJoint_spring.upperTranslation = PIXEL_TO_METERS(140);

	prismaticJoint_spring.localAnchorA.Set(0, 0);
	prismaticJoint_spring.localAnchorB.Set(0, 0);


	prismaticJoint_spring.localAxisA.Set(0, -1);

	b2PrismaticJoint* joint_spring = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJoint_spring);
}

void ModuleScenePinball::Map_shape()
{
	p2List_item<PhysBody*>* temp = chains.getFirst();
	while (temp != NULL) {
		b2Body* body = temp->data->body;
		temp->data->body->GetWorld()->DestroyBody(body);
		temp = temp->next;
	}
	chains.clear();

	chains.add(App->physics->CreateChain(0, 0, Background, 90,b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, ShapeL1, 12, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, ShapeR1, 12, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, ShapeR2, 12, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, ShapeL2, 10, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, SpinTube, 48, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, Start_Tube, 22, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, Circle1, 40, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, Circle2, 40, b2_staticBody));
}

void ModuleScenePinball::Ball_respawn()
{

	if (ball_y > 1400 && App->UI->num_balls >= 1)
	{

		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(592), PIXEL_TO_METERS( 845)), 0);	
		ball->body->SetAngularVelocity(0);
		ball->body->SetLinearVelocity(b2Vec2(0, 0));
		--App->UI->num_balls;
		
	}
}
void ModuleScenePinball::Sensed()
{
	
	if (blue_sensed)
	{
		App->UI->score += 100;
		blit_blue = true;
		ball->GetPosition(x, y);
		Create100(x, y);
		blue_sensed = !blue_sensed;
	}
	if (green_sensed)
	{
		App->UI->score += 100;
		blit_green = true;
		ball->GetPosition(x, y);
		Create100(x, y);
		green_sensed = !green_sensed;
	}
	if (orange_sensed)
	{
		App->UI->score += 100;
		blit_orange = true;
		ball->GetPosition(x, y);
		Create100(x, y);
		orange_sensed = !orange_sensed;
	}
	if (pink_sensed)
	{
		App->UI->score += 100;
		blit_pink = true;
		ball->GetPosition(x, y);
		Create100(x, y);
		pink_sensed = !pink_sensed;
	}
	if (yellow_sensed)
	{
		App->UI->score += 100;
		blit_yellow = true;
		ball->GetPosition(x, y);
		Create100(x, y);
		yellow_sensed = !yellow_sensed;
	}
	if (red_sensed)
	{
		App->UI->score += 100;
		blit_red = true;
		ball->GetPosition(x, y);
		Create100(x, y);
		red_sensed = !red_sensed;
	}


}
void ModuleScenePinball::Create100(int x, int y)
{
	plus100 = App->textures->Load("Textures/_100.png");
	x100 = x;
	y100 = y;
	start_timer = true;
	
}

