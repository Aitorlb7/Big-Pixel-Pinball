#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleScenePinball.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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
	App->renderer->camera.y = -0; //Magic number

	Map_shape();
	

	background = App->textures->Load("Textures/background.png");
	ball = App->textures->Load("Textures/ball.png");
	flipper_right_tex = App->textures->Load("Textures/Flipper_Right.png");

	//--------------------------------
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
	
	
	b2BodyDef body;
	body.type = b2_dynamicBody;
	//body.position.Set(PIXEL_TO_METERS(300), PIXEL_TO_METERS(560));

	b2Body* b =  App->physics->world->CreateBody(&body);
	b2PolygonShape box;

	b2Vec2* p = new b2Vec2[17/2];

	for (uint i = 0; i < 17 / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(Flipper_Right[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(Flipper_Right[i * 2 + 1]);
	}

	box.Set(p, 17 / 2);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = 0.2f;

	b->CreateFixture(&fixture);

	
	flipper_right_body->body = b;
	b->SetUserData(flipper_right_body);
	flipper_right_body->height = flipper_right_body->width = 0;

	//--------------------------------
	right_flipper_joint = App->physics->CreateCircle(215, 582, 2, b2_staticBody);

	b2RevoluteJointDef revoluteJointDef_right;
	revoluteJointDef_right.bodyA = right_flipper_joint->body;
	revoluteJointDef_right.bodyB = flipper_right_body->body;
	revoluteJointDef_right.collideConnected = false;

	revoluteJointDef_right.enableLimit = true;
	revoluteJointDef_right.lowerAngle = -25 * DEGTORAD;
	revoluteJointDef_right.upperAngle = 25 * DEGTORAD;

	revoluteJointDef_right.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef_right.localAnchorB.Set(PIXEL_TO_METERS(10), PIXEL_TO_METERS(15));

	b2RevoluteJoint* right_flipper_joint;
	right_flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_right);

	//--------------------------------
	Soundtrack[0] = App->audio->LoadFx("Audio/MainTheme.wav");
	//Soundtrack[1] = App->audio->LoadFx("Audio/MenuTheme.wav");
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

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		//App->audio->PlayFx(flipper_sound, 0);
		flipper_right_body->body->ApplyTorque(300, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipper_right_body->body->ApplyTorque(100, true);
	}
	else
	{
		flipper_right_body->body->ApplyTorque(-50, true);
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		//circles.getLast()->data->listener = this;

		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15, b2_dynamicBody));
		balls.getLast()->data->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------
	

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* b = balls.getFirst();

	while(b != NULL)
	{
		int x, y;
		b->data->GetPosition(x, y);
		if(b->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(ball, x, y, NULL, 1.0f, b->data->GetRotation());
		b = b->next;
	}

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


	App->renderer->Blit(flipper_right_tex, 100, 100, NULL, 1.0f, flipper_right_body->GetRotation(), 55, 10);
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

void ModuleScenePinball::Map_shape()
{
	p2List_item<PhysBody*>* temp = chains.getFirst();
	while (temp != NULL) {
		b2Body* body = temp->data->body;
		temp->data->body->GetWorld()->DestroyBody(body);
		temp = temp->next;
	}
	chains.clear();

	
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
	chains.add(App->physics->CreateChain(0, 0, Background, 82,b2_staticBody));
}
