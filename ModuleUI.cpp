#include "ModuleUI.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include <string>

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Banner = { 0,479,630,48 };
	Scorerect1 = {14,528, 116, 16};
	Scorerect2 = {14,543, 116, 16};
	Inforect = { 152,486, 340, 35 };

}
ModuleUI::~ModuleUI()
{
}
bool ModuleUI::Start()
{
	LOG("Loading UI textures");
	graphics = App->textures->Load("Textures/Assets_Pinball.png");
	font_start = App->fonts->Load("Textures/Font.png", "0123456789", 1);
	return true;
}
update_status ModuleUI::Update()
{
	y = -App->renderer->camera.y;

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT) // suma de puntos
	{
		score++;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)  // en caso de que la bola caiga
	{
		if (num_balls == 3)
		{
			high_score = score;
		}
		num_balls--;
		prev_score = score;
		if (score > high_score)
		{
			high_score = score;
		}
		score = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) // Reiniciar nivel
	{
		num_balls = 3;
	}




	//RENDERS----------------------------------------------------------------
	
	App->renderer->Blit(graphics, 0, y, &Banner);
	App->fonts->BlitText(5, y + 23, 0, std::to_string(score).c_str());
	App->fonts->BlitText(540, y + 23, 0, std::to_string(num_balls).c_str());
	if (num_balls <= 0)
	{
		App->renderer->DrawQuad({ 170, y + 200, 300, 150 }, 0, 0, 0, 240, true, true);
		App->renderer->Blit(graphics, 190, y + 220, &Scorerect1);
		App->renderer->Blit(graphics, 190, y + 280, &Scorerect2);
		App->renderer->Blit(graphics, 152, y + 5, &Inforect);
		App->fonts->BlitText(190, y + 240, 0, std::to_string(high_score).c_str());
		App->fonts->BlitText(190, y + 300, 0, std::to_string(prev_score).c_str());
	}
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	if (graphics != nullptr) { 
		SDL_DestroyTexture(graphics);
		graphics = nullptr;
	}
	return true;
}