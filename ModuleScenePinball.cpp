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
	sensed = false;
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



	Soundtrack = App->audio->LoadFx("Audio/MainTheme.wav");
	launcher_fx = App->audio->LoadFx("Audio/Launcher_fx.wav");
	flipper_fx = App->audio->LoadFx("Audio/Flipper_fx.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

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
	App->renderer->Blit(background, 0, 0);
	
	static int force = 0;
	static int elastic_force = 1;
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

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		//circles.getLast()->data->listener = this;

		balls.add(App->physics->CreateCircle(592, 845, 15, b2_dynamicBody));
		balls.getLast()->data->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------
	

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* b = balls.getFirst();

	//while(b != NULL)
	//{
	//	int x, y;
	//	b->data->GetPosition(x, y);
	//	App->renderer->Blit(ball_tex, x, y, NULL, 1.0f);
	//	b = b->next;
	//}

	b = boxes.getFirst();

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
	App->renderer->Blit(ball_tex, ball_x, ball_y, NULL, 1.0f);

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

	App->audio->PlayMusic("Audio/MainTheme.wav", -1.0f);
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
	//chains.add(App->physics->CreateChain(0, 0, ShapeL2, 12, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, SpinTube, 48, b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, Start_Tube, 22, b2_staticBody));
}

void ModuleScenePinball::Ball_respawn()
{

	if (ball_y > 1400 && App->UI->num_balls > 0)
	{

		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(592), PIXEL_TO_METERS( 845)), 0);	
		ball->body->SetAngularVelocity(0);
		ball->body->SetLinearVelocity(b2Vec2(0, 0));
		--App->UI->num_balls;
		
	}
}

