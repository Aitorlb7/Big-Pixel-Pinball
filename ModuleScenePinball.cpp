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
	
	
	//circle = App->textures->Load("pinball/wheel.png"); 
	//box = App->textures->Load("pinball/crate.png");
	//rick = App->textures->Load("pinball/rick_head.png");
	//bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	background = App->textures->Load("Textures/background.png");
	ball = App->textures->Load("Textures/ball.png");

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
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		//circles.getLast()->data->listener = this;

		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15));
		balls.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	//p2List_item<PhysBody*>* b = circles.getFirst();
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
		if(ray_on)
		{
			int hit = b->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		b = b->next;
	}

	b = ricks.getFirst();

	while(b != NULL)
	{
		int x, y;
		b->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, b->data->GetRotation());
		b = b->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	App->renderer->Blit(background, 0, 0);

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

void Map_shape()
{
	//p2List_item<PhysBody*>* temp = ricks.getFirst();
	//while (temp != NULL) {
	//	b2Body* body = temp->data->body;
	//	temp->data->body->GetWorld()->DestroyBody(body);
	//	temp = temp->next;
	//}
	//ricks.clear();

	int Background[148] = {
	640, 767,
	559, 739,
	556, 733,
	558, 727,
	581, 686,
	566, 676,
	564, 672,
	565, 667,
	606, 592,
	602, 590,
	601, 549,
	612, 549,
	611, 358,
	603, 357,
	603, 314,
	611, 313,
	611, 81,
	605, 88,
	592, 78,
	585, 84,
	552, 56,
	555, 48,
	546, 39,
	555, 29,
	426, 31,
	412, 45,
	418, 56,
	385, 84,
	374, 73,
	355, 90,
	352, 178,
	342, 190,
	331, 186,
	316, 156,
	274, 136,
	263, 126,
	259, 98,
	246, 75,
	224, 52,
	183, 33,
	164, 29,
	146, 28,
	126, 31,
	110, 37,
	95, 45,
	83, 54,
	69, 69,
	55, 95,
	50, 122,
	50, 157,
	50, 653,
	55, 679,
	62, 694,
	74, 709,
	82, 722,
	82, 742,
	82, 833,
	75, 836,
	61, 833,
	42, 834,
	41, 1079,
	48, 1078,
	58, 1083,
	194, 1157,
	211, 1173,
	205, 1361,
	380, 1357,
	400, 1173,
	421, 1155,
	562, 1078,
	563, 1176,
	633, 1173,
	640, 773,
	669, 272
	};
}
